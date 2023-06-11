#include <args.h>
#include <lib.h>

#define WHITESPACE " \t\r\n"
#define SYMBOLS "<|>&;()"

/* Overview:
 *   Parse the next token from the string at s.
 *
 * Post-Condition:
 *   Set '*p1' to the beginning of the token and '*p2' to just past the token.
 *   Return:
 *     - 0 if the end of string is reached.
 *     - '<' for < (stdin redirection).
 *     - '>' for > (stdout redirection).
 *     - '|' for | (pipe).
 *     - 'w' for a word (command, argument, or file name).
 *
 *   The buffer is modified to turn the spaces after words into zero bytes ('\0'), so that the
 *   returned token is a null-terminated string.
 */
int _gettoken(char *s, char **p1, char **p2) {
	*p1 = 0;
	*p2 = 0;
	if (s == 0) {
		return 0;
	}

	while (strchr(WHITESPACE, *s)) {
		*s++ = 0;
	}
	if (*s == 0) {
		return 0;
	}

	if (*s == '"') {
		s++;
		*p1 = s;
		while (*s && *s != '"') {
			s++;
		}
		*s = 0;
		*p2 = s;
		return 'w';
	}

	if (strchr(SYMBOLS, *s)) {
		int t = *s;
		*p1 = s;
		*s++ = 0;
		*p2 = s;
		return t;
	}

	*p1 = s;
	while (*s && !strchr(WHITESPACE SYMBOLS, *s)) {
		s++;
	}
	*p2 = s;
	return 'w';
}

int gettoken(char *s, char **p1) {
	static int c, nc;
	static char *np1, *np2;

	if (s) {
		nc = _gettoken(s, &np1, &np2);
		return 0;
	}
	c = nc;
	*p1 = np1;
	nc = _gettoken(np2, &np1, &np2);
	return c;
}

#define MAXARGS 128

int parsecmd(char **argv, int *rightpipe) {
	int argc = 0;
	while (1) {
		char *t;
		int fd, r;
		int c = gettoken(0, &t);
		switch (c) {
		case 0:
			return argc;
		case 'w':
			if (argc >= MAXARGS) {
				debugf("too many arguments\n");
				exit();
			}
			argv[argc++] = t;
			break;
		case '<':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: < not followed by word\n");
				exit();
			}
			// Open 't' for reading, dup it onto fd 0, and then close the original fd.
			fd = open(t, O_RDONLY);
			dup(fd, 0);
			close(fd);
			break;
		case '>':
			if (gettoken(0, &t) != 'w') {
				debugf("syntax error: > not followed by word\n");
				exit();
			}
			// Open 't' for writing, dup it onto fd 1, and then close the original fd.
			fd = open(t, O_WRONLY);
			dup(fd, 1);
			close(fd);
			break;
		case '|':;
			/*
			 * First, allocate a pipe.
			 * Then fork, set '*rightpipe' to the returned child envid or zero.
			 * The child runs the right side of the pipe:
			 * - dup the read end of the pipe onto 0
			 * - close the read end of the pipe
			 * - close the write end of the pipe
			 * - and 'return parsecmd(argv, rightpipe)' again, to parse the rest of the
			 *   command line.
			 * The parent runs the left side of the pipe:
			 * - dup the write end of the pipe onto 1
			 * - close the write end of the pipe
			 * - close the read end of the pipe
			 * - and 'return argc', to execute the left of the pipeline.
			 */
			int p[2];
			pipe(p);
			if ((*rightpipe = fork()) == 0) {
				dup(p[0], 0);
				close(p[0]);
				close(p[1]);
				return parsecmd(argv, rightpipe);
			} else {
				dup(p[1], 1);
				close(p[0]);
				close(p[1]);
				return argc;
			}
			break;
		case ';':;
			int left = fork();
			if (left > 0) {
				wait(left);
				return parsecmd(argv, rightpipe);
			} else {
				return argc;
			}
			break;
		case '&':;
			int back = fork();
			if (back == 0) {
				return argc;
			} else {
				return parsecmd(argv, rightpipe);
			}
			break;
		}
	}

	return argc;
}

void runcmd(char *s) {
	gettoken(s, 0);

	char *argv[MAXARGS];
	int rightpipe = 0;
	int argc = parsecmd(argv, &rightpipe);
	if (argc == 0) {
		return;
	}
	argv[argc] = 0;
	if (rightpipe) {
		wait(rightpipe);
	}
	int child = spawn(argv[0], argv);
	close_all();
	if (child >= 0) {
		wait(child);
	} else {
		debugf("spawn %s: %d\n", argv[0], child);
	}
	
	exit();
}

/****************************************************
 * readLine
 * beforeCur from 0 to beforeLen-1
 * afterCur from afterLen-1 to 0
 * but cant intput 27 ESC
 * 暂时不支持delete
*****************************************************/
#define UP 0x4b00
#define DOWN 0x4d00
#define RIGHT 0x5000
#define LEFT 0x4800
#define DEL 0x4000

#define printUp printf("%c%c%c", 27, 91, 65);
#define printDown printf("%c%c%c", 27, 91, 66);
#define printRight printf("%c%c%c", 27, 91, 67);
#define printLeft printf("%c%c%c", 27, 91, 68);

int inputLen, beforeLen, afterLen;
char beforeCur[1024];
char afterCur[1024];
char back[1024];
char space[1024];

void initBS() {
	int i;
	if (back[0] == '\b' || space[0] == ' ') {
		return;
	}
	for (i = 0; i < 1024; i++) {
		back[i] = '\b';
	}
	for (i = 0; i < 1024; i++) {
		space[i] = ' ';
	}
}
void printBack(int num) {
	back[num] = 0;
	printf("%s", back);
	back[num] = '\b';
}
void printSpace(int num) {
	space[num] = 0;
	printf("%s", space);
	space[num] = ' ';
}

void updateCons(int cursor) {
	int i;
	printBack(cursor);
	printSpace(inputLen+1);
	printBack(inputLen+1);

	for (i = 0; i < beforeLen; i++){
		printf("%c", beforeCur[i]);
	}
	for (i = afterLen-1; i >= 0; i--) {
		printf("%c", afterCur[i]);
	}
	for (i = 0; i < afterLen; i++){
		printLeft;
	}
}

void backOrDelete(int op) {
	if (inputLen == 0) {
		return;
	} else if (op == DEL) {
		if (afterLen == 0) {
			return;
		}
		afterLen--;
		inputLen--;
		updateCons(beforeLen);
	} else {
		if (beforeLen == 0) {
			return;
		}
		beforeLen--;
		inputLen--;
		if (afterLen == 0) {
			printf("\b \b");
		} else {
			updateCons(beforeLen+1);
		}
	}
}

int readDir() {
	char inc = 0;
	int r = 0;
	r = read(0, &inc, 1);
	if (inc == 91) {
		r = read(0, &inc, 1);
		switch (inc) {
			case 'A':
				return UP;
			case 'B':
				return DOWN;
			case 'C':
				return RIGHT;
			case 'D':
				return LEFT;
			case '3':
				r = read(0, &inc, 1);
				if (inc == 126) {
					return DEL;
				}
			default:
				debugf("read error: %d\n", r);
				exit();
		}
	} else {
		debugf("read error: %d\n", r);
		exit();
	}
}

void moveCursor(int direction) {
	if (direction == UP) {
		printDown;
		return;
	} else if (direction == DOWN) {
		return;
	} else if (beforeLen == inputLen 
				&& direction == RIGHT) {
		printLeft;
		return;
	} else if (beforeLen == 0 
				&& direction == LEFT) {
		printRight;
		return;
	}
	
	if (direction == RIGHT) {
		beforeCur[beforeLen++] = afterCur[--afterLen];
	} else if (direction == LEFT) {
		afterCur[afterLen++] = beforeCur[--beforeLen];
	}
}

void loadBuf(char *buf) {
	int i;
	int bufp = 0;
	for (i = 0; i < beforeLen; i++){
		buf[bufp++] = beforeCur[i];
	}
	for (i = afterLen-1; i >= 0; i--) {
		buf[bufp++] = afterCur[i];
	}
	buf[bufp] = 0;
}

void readline(char *buf, u_int n) {
	int r;
	beforeLen = 0;
	afterLen = 0;
	for (inputLen = 0; inputLen < n;) {
		char inc = 0;
		int dir = 0;
		if ((r = read(0, &inc, 1)) != 1) {
			if (r < 0) {
				debugf("read error: %d\n", r);
			}
			exit();
		}
		// printf("\n%d", inc);

		if (inc == 27) {
			dir = readDir();
		}
		if (dir == DEL) {
			backOrDelete(dir);
		}
		
		if (inc == '\r' || inc == '\n') {
			loadBuf(buf);
			return;
		} else if (inc == '\b' || inc == 0x7f) {
			backOrDelete(inc);
		} else if (inc == 27) {
			moveCursor(dir);
		} else {
			beforeCur[beforeLen++] = inc;
			inputLen++;
			if (afterLen != 0) {
				updateCons(beforeLen);
			}
		}
	}
	debugf("line too long\n");
	while ((r = read(0, buf, 1)) == 1 && buf[0] != '\r' && buf[0] != '\n') {
		;
	}
	buf[0] = 0;
}

/****************************************************
 * readLine
 * End
*****************************************************/

char buf[1024];

void usage(void) {
	debugf("usage: sh [-dix] [command-file]\n");
	exit();
}

int main(int argc, char **argv) {
	int r;
	int interactive = iscons(0);
	int echocmds = 0;
	initBS();
	debugf("\n:::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	debugf("::                                                         ::\n");
	debugf("::                     MOS Shell 2023                      ::\n");
	debugf("::                                                         ::\n");
	debugf(":::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::\n");
	ARGBEGIN {
	case 'i':
		interactive = 1;
		break;
	case 'x':
		echocmds = 1;
		break;
	default:
		usage();
	}
	ARGEND

	if (argc > 1) {
		usage();
	}
	if (argc == 1) {
		close(0);
		if ((r = open(argv[1], O_RDONLY)) < 0) {
			user_panic("open %s: %d", argv[1], r);
		}
		user_assert(r == 0);
	}
	for (;;) {
		if (interactive) {
			char *pathName;
			syscall_get_env_path(pathName);
			printf("\n%s$ ", pathName);
		}
		readline(buf, sizeof buf);

		if (buf[0] == '#') {
			continue;
		}
		if (echocmds) {
			printf("# %s\n", buf);
		}
		if ((r = fork()) < 0) {
			user_panic("fork: %d", r);
		}
		if (r == 0) {
			runcmd(buf);
			exit();
		} else {
			wait(r);
		}
	}
	return 0;
}
