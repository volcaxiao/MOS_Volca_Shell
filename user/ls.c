#include <lib.h>
#define _OP_ERR_                                                                                    \
	"ls: missing operand \n\
Try 'ls --help' for more information. \n"

#define _HELP_                                                                                     \
	"Usage: ls [OPTION]... [FILE]... \n\
List information about the FILEs (the current directory by default). \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -a, --all                  do not ignore entries starting with . \n\
  -d, --directory            list directories themselves, not their contents \n\
  -F, --classify             append indicator (one of */=>@|) to entries \n\
  -l                         use a long listing format \n\
      --help     display this help and exit \n"

int flag[256];

void lsdir(char *, char *);
void ls1(char *, u_int, u_int, char *);

void ls(char *path, char *prefix) {
	int r;
	struct Stat st;

	if ((r = stat(path, &st)) < 0) {
		user_panic("stat %s: %d", path, r);
	}
	if (st.st_isdir && !flag['d']) {
		lsdir(path, prefix);
	} else {
		ls1(0, st.st_isdir, st.st_size, path);
	}
}

void lsdir(char *path, char *prefix) {
	int fd, n;
	struct File f;

	if ((fd = open(path, O_RDONLY)) < 0) {
		user_panic("open %s: %d", path, fd);
	}
	while ((n = readn(fd, &f, sizeof f)) == sizeof f) {
		if (f.f_name[0]) {
			ls1("", f.f_type == FTYPE_DIR, f.f_size, f.f_name);
		}
	}
	if (n > 0) {
		user_panic("short read in directory %s", path);
	}
	if (n < 0) {
		user_panic("error reading directory %s: %d", path, n);
	}
}

void ls1(char *prefix, u_int isdir, u_int size, char *name) {
	char *sep;
	if (!flag['a'] && name[0] == '.') {
		return;
	}

	if (flag['l']) {
		printf("0x%-8x %c ", size, isdir ? 'd' : '-');
	}
	if (prefix) {
		if (prefix[0] && prefix[strlen(prefix) - 1] != '/') {
			sep = "/";
		} else {
			sep = "";
		}
		printf("%s%s", prefix, sep);
	}
	if (isdir) {
		printf("\033[34;1m%s\033[0m", name);
	} else {
		printf("%s", name);
	}

	if (flag['F'] && isdir) {
		printf("/");
	}
	if (flag['l']) {
		printf("\n");
	} else {
		printf(" ");
	}
}

void usage(void) {
	printf(_OP_ERR_);
	exit();
}

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		debugf(_HELP_);
		return 0;
	}
	
	int i;

	ARGBEGIN {
	default:
		usage();
	case 'a':
	case 'd':
	case 'F':
	case 'l':
		flag[(u_char)ARGC()]++;
		break;
	}
	ARGEND

	if (argc == 0) {
		char nowPath[MAXPATHLEN];
		syscall_get_env_path(0, nowPath);
		ls(nowPath, "");
	} else {
		for (i = 0; i < argc; i++) {
			ls(argv[i], argv[i]);
		}
	}
	printf("\n");
	return 0;
}
