#include <lib.h>
#define _OP_ERR_ "declare: missing operand \n\
Try 'declare --help' for more information. \n"

#define _HELP_                                                                                     \
	"Usage: declare [-xr] [NAME [=VALUE]] \n\
Set environment variables or print all environment variables. \n\
By default, Print all environment variables \n\
If there are other parameters, as follow. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -x            Set global environment variables \n\
  -r            Environment variables are set to be readable only \n\
  -q, --quiet   no error send if error \n\
  -v, --verbose print a message for each create \n\
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
        case 'x':
        case 'r':
        case 'q':
        case 'v':
            flag[(u_char)ARGC()]++;
            break;
        }
	ARGEND
    if (argc > 2) {
        debugf(_OP_ERR_);
        return -1;
    }

    int isGlobal = 0;
    int perm = VAR_RW;
    if (flag['x']) {
        isGlobal = 1;
    }
    if (flag['r']) {
        perm = VAR_RDONLY;
    }

    if (argc == 0) {
        struct Var buf[1024];
        int cnt = syscall_get_all_var(buf);
        int i = 0;
        for (; i < cnt; i++) {
            printf("%s = %s\n", buf[i].name, buf[i].value);
        }
    } else {
        char *value = "";
        if (argc == 2) {
            if (argv[1][0] == '=') {
                value = argv[1] + 1;
            }
        }
        // printf("get var %s\n", value);
        int r = syscall_declare_var(argv[0], value, perm, isGlobal);
        if (r < 0) {
            if (!flag['q']) {
                if (r == -1) {
                    debugf("The var write error\n");
                } else if (r == -2) {
                    debugf("The var is readOnly!\n");
                }
            }
        } else {
            if (flag['v']) {
                debugf("write successful!\n");
            }
        }
    }
    return 0;
}