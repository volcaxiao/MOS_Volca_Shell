#include <lib.h>
#define _OP_ERR_ ""
#define _HELP_ ""

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		debugf(_HELP_);
		return 0;
	}
}