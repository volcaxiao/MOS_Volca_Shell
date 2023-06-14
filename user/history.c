#include <lib.h>
#define MAX_HIS_NUM 1024

char *history_buf[MAX_HIS_NUM];

int main(int argc, char **argv) {
    if (argc == 1) {
        int len = history_get_all(history_buf);
        int i = 0;
        printf("total history cmd num is %d\n", len);
        for (i = 0; i < len; i++) {
            printf ("%4d : %s\n", i + 1, history_buf[i]);
        }
    }
    return 0;
}