#include <lib.h>
#define _OP_ERR_ "unset: missing operand \n\
Try 'unset --help' for more information. \n"

#define _HELP_                                                                                     \
	"Usage: declare [-xr] [NAME [=VALUE]] \n\
Unset environment variables. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -q, --quiet   no error send if error \n\
  -v, --verbose print a message for each unset \n\
     --help     display this help and exit \n"

int flag[256];

void usage(void) {
	printf(_OP_ERR_);
	exit();
}

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		debugf(_HELP_);
		return 0;
	}
    ARGBEGIN {
        default:
            usage();
        case 'q':
        case 'v':
            flag[(u_char)ARGC()]++;
            break;
    }
	ARGEND
    if (argc != 1) {
        debugf(_OP_ERR_);
        return -1;
    }
    int r = syscall_unset_var(argv[0]);
    if (r == 0) {
        if (flag['v']) {
            debugf("unset successful!\n");
        }
    } else if (r == -1 && !flag['q']) {
        debugf("The var is no exist!\n");
    } else if (r == -2 && !flag['q']) {
        debugf("The var is readOnly!\n");
    }
    return 0;
}