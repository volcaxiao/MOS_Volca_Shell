#include <lib.h>

void usage() {
	debugf("usage: cd [dir]\n");
}

int cdAT(char *path) {
    char nowPath[MAXPATHLEN];
    syscall_get_env_path(0, nowPath);
    int fd = openatThis(nowPath, path, O_RDONLY);
    if (fd < 0) {
        debugf("this path is no exist\n");
		usage();
		return fd;
    } else {
        struct File file = ((struct Filefd *)num2fd(fd))->f_file;
		if (file.f_type == FTYPE_DIR) {
			syscall_change_dir(nowPath, 1);
		} else {
			debugf("this path is a file\n");
			return -1;
		}
    }
    return 0;
}

int cdAP(char *path) {
	int fd = openAP(path, O_RDONLY);
	if (fd < 0) {
		debugf("this path is no exist\n");
		usage();
		return fd;
	} else {
		struct File file = ((struct Filefd *)num2fd(fd))->f_file;
		if (file.f_type == FTYPE_DIR) {
			syscall_change_dir(path, 1);
		} else {
			debugf("this path is a file\n");
			return -1;
		}
	}
    return 0;
}

int main(int argc, char **argv) {
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