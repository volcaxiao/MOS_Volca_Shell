#include <lib.h>
#define _OP_ERR_ "error"
#define _HELP_ ""

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
        }
	ARGEND
    if (argc != 1) {
        debugf(_OP_ERR_);
        return -1;
    }
    int r = syscall_unset_var(argv[0]);
    if (r == 0) {

    } else if (r == -1) {
        debugf("The var is no exist!\n");
    } else if (r == -2) {
        debugf("The var is readOnly!\n");
    }
    return 0;
}