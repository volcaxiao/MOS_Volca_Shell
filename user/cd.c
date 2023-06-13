#include <lib.h>

void usage() {
    debugf("usage: cd [dir]\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        syscall_change_dir("", 1);
    } else {
        int fd;
        fd = open(argv[1], O_RDONLY);
        if (fd < 0) {
            debugf("this path is no exist\n");
            usage();
            return -1;
        } else {
            struct File file = ((struct Filefd *)num2fd(fd))->f_file;
            if (file.f_type == FTYPE_DIR) {
                syscall_change_dir(argv[1], 1);
            } else {
                debugf("this path is a file\n");
                return -1;
            }
        }
    }
    return 0;
}