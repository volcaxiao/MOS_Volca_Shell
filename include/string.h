#ifndef _STRING_H_
#define _STRING_H_

#include <types.h>

void *memcpy(void *dst, const void *src, size_t n);
void *memset(void *dst, int c, size_t n);
size_t strlen(const char *s);
char *strcpy(char *dst, const char *src);
const char *strchr(const char *s, int c);
int strcmp(const char *p, const char *q);
char *strcat(char *dst, const char *src);
int strContain(const char *parent, const char *child, char *waitToComp);

char *catPath(char *pre, const char *path);
char *toParentPath(char *pathName);
// char *pathNameFormat(const char *pathName);
int splitPath(char *firDir, char* nextPath, const char *pathName);

int strToUint(char *str);

#endif
