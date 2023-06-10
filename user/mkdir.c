#include <lib.h>

void usage() {
    debugf("usage: mkdir [command-dir]\n");
}

int main(int argc, char **argv) {
    if (argc == 1) {
        usage();
        return -1;
    } else {
        int i;
		for (i = 1; i < argc; i++) {
            int fd;
			fd = open(argv[i], O_MKDIR);
			if (fd < 0) {
				user_panic("can't mkdir %s: %d", argv[i], fd);
			} else {
				close(fd);
			}
		}
    }
    return 0;
}