#include <lib.h>

int nowOff;
int maxOff;
int fakeMaxOff;

void history_init() {
    int fd = openAP(".history", O_CREAT);
    if (fd < 0) {
        user_panic("history init fail!\n");
    }
    nowOff = 0;
    maxOff = 0;
    fakeMaxOff = 0;
    close(fd);
}

int writeEntry(int fd, int offset, const char* buf) {
    int len = strlen(buf);
    seek(fd, offset);
    write(fd, &len, 4);
    write(fd, buf, len);
    write(fd, &len, 4);
    // 标定结束
    int zero = 0;
    write(fd, &zero, 4);
    write(fd, &zero, 4);
    // printf("\n%s\n", buf);
    return len;
}

int readDownEntry(int fd, int offset, char* buf) {
    seek(fd, offset);
    int len;
    read(fd, &len, 4);
    read(fd, buf, len);
    buf[len] = 0;
    //printf("\n%s\n", buf);
    return len;
}

int readUpEntry(int fd, int offset, char* buf) {
    seek(fd, offset - 4);
    int len;
    read(fd, &len, 4);
    seek(fd, offset - 4 - len);
    read(fd, buf, len);
    buf[len] = 0;
    // printf("\n%s\n", buf);
    return len;
}

/**
 * return 0 if success;
*/
int history_write(const char* buf) {
    if (buf[0] == 0) {
        return -1;
    }
    int fd = openAP(".history", O_WRONLY);
    if (fd < 0) {
        return fd;
    }
    int len = writeEntry(fd, maxOff, buf);
    maxOff += len + 8;
    nowOff = maxOff;
    fakeMaxOff = maxOff;
    close(fd);
    // printf ("now MaxOff is %d\n", maxOff);
    return 0;
}

/**
 * return cnt of history;
*/
char tmpBuf[1024][MAX_CMD_LEN];
int history_get_all(char **history_buf) {
    // printf ("now MaxOff is %d\n", maxOff);
    int fd = openAP(".history", O_RDONLY);
    nowOff = 0;
    int cnt = 0;
    while (1) {
        int len = readDownEntry(fd, nowOff, tmpBuf[cnt]);
        history_buf[cnt] = tmpBuf[cnt];
        // printf ("%4d : %s\n", cnt + 1, history_buf[cnt]);
        if (len == 0) {
            break;
        }
        nowOff += len + 8;
        cnt++;
    }
    close(fd);
    return cnt;
}

/**
 * direction: 0 is down, 1 is up;
 * return < 0 if direction error
 * nowOff is 0 but up
 * nowOff is maxOff buf down
 * return len;
*/
int history_next(char* nowBuf, char* buf, int direction) {
    // printf ("now MaxOff is %d\n", maxOff);
    if ((nowOff == 0 && direction == 1) || maxOff == 0) {
        return -1;
    }
    if (nowOff == maxOff && direction == 0 && maxOff == fakeMaxOff) {
        return 0;
    }

    int fd = openAP(".history", O_RDWR);
    if (nowOff == maxOff && direction == 1) {
        int len = writeEntry(fd, maxOff, nowBuf);
        fakeMaxOff = maxOff + len + 8;
    }
    
    if (fd < 0) {
        return fd;
    }
    int len;
    if (direction == 0) {
        if (nowOff < maxOff) {
            int len = readDownEntry(fd, nowOff, buf);
            nowOff += len + 8;
        }
        len = readDownEntry(fd, nowOff, buf);
    } else {
        len = readUpEntry(fd, nowOff, buf);
        nowOff -= (len + 8);
    }
    close(fd);
    
    return len;
}
