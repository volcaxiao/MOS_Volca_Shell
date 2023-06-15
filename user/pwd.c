#include <lib.h>
#define _OP_ERR_ "Mos's pwd does not support any parameters, but \'--help\'\n"
#define _HELP_ "Usage: pwd \n\
Print the name of the current working directory. Mos's pwd does not support any parameters. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
      --help        display this help and exit \n"

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		debugf(_HELP_);
		return 0;
	}
	if (argc == 1) {
		char pathName[1024];
		syscall_get_env_path(0, pathName);
		printf("Now the absolute path is\n    \033[34;1m%s\033[0m", pathName);
	} else {
		debugf(_OP_ERR_);
	}
}