#include <lib.h>
#define _OP_ERR_                                                                                    \
	"touch: missing operand \n\
Try 'touch --help' for more information. \n"

#define _HELP_                                                                                     \
	"Usage: touch [OPTION]... FILE... \n\
Mos's touch only supports the following parameters and is only used for creating files \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -q, --quiet       no error if existing \n\
  -v, --verbose     print a message for each created directory \n\
      --help        display this help and exit \n"

int flag[256];
void usage() {
	debugf(_OP_ERR_);
}

void touch(char *pathName) {
	int fd;
	fd = open(pathName, O_RDONLY);
	if (fd > 0) {
        close(fd);
        if (!flag['q']) {
            debugf("touch: cannot create file \'%s\': File exists\n", pathName);
        }
        return;
	}
    fd = open(pathName, O_CREAT);
	if (fd < 0) {
        if (!flag['q']) {
            user_panic("touch: cannot create %s in some error\nerror code: %d", pathName, fd);
        }
	} else {
        if (flag['v']) {
            debugf("touch: created file \'%s\'\n", pathName);
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
			touch(argv[i]);
		}
	}
	return 0;
}