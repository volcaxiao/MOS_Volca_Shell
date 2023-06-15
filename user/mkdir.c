#include <lib.h>

#define _HELP_ "Usage: mkdir [OPTION]... DIRECTORY... \n\
Create the DIRECTORY(ies), if they do not already exist. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -q, --quiet       no error if existing \n\
  -v, --verbose     print a message for each created directory \n\
      --help        display this help and exit \n"

#define _OP_ERR_ "mkdir: missing operand \n\
Try 'mkdir --help' for more information. \n"

int flag[256];

void usage() {
    debugf(_OP_ERR_);
}

void mkdir(char *dirPath) {
    int fd;
    fd = open(dirPath, O_RDONLY);
    if (fd > 0) {
        close(fd);
        if (!flag['q']) {
            debugf("mkdir: cannot create directory \'%s\': File exists\n", dirPath);
        }
        
        return;
    }
    fd = open(dirPath, O_MKDIR);
	if (fd < 0) {
        if (!flag['q']) {
            user_panic("mkdir: cannot create %s in some error\nerror code: %d", dirPath, fd);
        }
	} else {
        if (flag['v']) {
            debugf("mkdir: created directory \'%s\'\n", dirPath);
        }
		close(fd);
	}
}

int main(int argc, char **argv) {
    if (strcmp(argv[1], "--help") == 0) {
        debugf(_HELP_);
        return 0;
    }
	ARGBEGIN {
	case 'q':
	case 'v':
		flag[(u_char)ARGC()]++;
		break;
	}
	ARGEND

    if (argc == 0) {
        usage();
        return -1;
    } else {
        int i;
		for (i = 0; i < argc; i++) {
            mkdir(argv[i]);
		}
    }
    return 0;
}