#include <lib.h>
#define _OP_ERR_ "cd: missing operand \n\
Try 'cd --help' for more information. \n"

#define _HELP_ "Usage: cd [dir]\n\
Change the shell working directory. \n\
Change the current directory to DIR.  The default DIR is the value of the root dir. \n\
You can use \".\" to represent the current path, use \"..\" to represent the parent path. \n\
Mos's cd does not support any parameters. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
      --help        display this help and exit \n"

void usage() {
	debugf(_OP_ERR_);
}

int cdAT(char *path) {
	char nowPath[MAXPATHLEN];
	syscall_get_env_path(0, nowPath);
	int fd = openatThis(nowPath, path, O_RDONLY);
	if (fd < 0) {
		debugf("This path: \'%s\' is no exist\n", path);
		usage();
		return fd;
	} else {
		struct File file = ((struct Filefd *)num2fd(fd))->f_file;
		if (file.f_type == FTYPE_DIR) {
			syscall_change_dir(nowPath, 1);
		} else {
			debugf("This path: \'%s\' is a file\n", path);
			usage();
			return -1;
		}
	}
	return 0;
}

int cdAP(char *path) {
	int fd = openAP(path, O_RDONLY);
	if (fd < 0) {
		debugf("This path: \'%s\' is no exist\n", path);
		usage();
		return fd;
	} else {
		struct File file = ((struct Filefd *)num2fd(fd))->f_file;
		if (file.f_type == FTYPE_DIR) {
			syscall_change_dir(path, 1);
		} else {
			debugf("This path: \'%s\' is a file\n", path);
			usage();
			return -1;
		}
	}
	return 0;
}

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		printf(_HELP_);
		return 0;
	}
	if (argc == 1) {
		syscall_change_dir("/", 1);
	} else {
		char *pathName = argv[1];
		if (pathName[0] == '/') {
			return cdAP(pathName);
		} else {
			return cdAT(pathName);
		}
	}
	return 0;
}