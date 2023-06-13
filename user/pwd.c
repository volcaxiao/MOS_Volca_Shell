#include <lib.h>

int main(int argc, char **argv) {
    if (argc == 1) {
        char pathName[1024];
        syscall_get_env_path(0, pathName);
        printf("%s\n", pathName);
    }
}