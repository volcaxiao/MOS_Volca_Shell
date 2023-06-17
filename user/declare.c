#include <lib.h>
#define _OP_ERR_ "err"
#define _HELP_ "help"

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
        syscall_declare_var(argv[0], value, perm, isGlobal);
    }
    return 0;
}