#include <types.h>
#define isAlphaOrData(c) (('a' <= c && c <= 'z') \
						|| ('A' <= c && c <= 'Z') \
						|| ('0' <= c && c <= '9'))
#define MAXPATHLEN 1024
#define MAXNAMELEN 128

void *memcpy(void *dst, const void *src, size_t n) {
	void *dstaddr = dst;
	void *max = dst + n;

	if (((u_long)src & 3) != ((u_long)dst & 3)) {
		while (dst < max) {
			*(char *)dst++ = *(char *)src++;
		}
		return dstaddr;
	}

	while (((u_long)dst & 3) && dst < max) {
		*(char *)dst++ = *(char *)src++;
	}

	// copy machine words while possible
	while (dst + 4 <= max) {
		*(uint32_t *)dst = *(uint32_t *)src;
		dst += 4;
		src += 4;
	}

	// finish the remaining 0-3 bytes
	while (dst < max) {
		*(char *)dst++ = *(char *)src++;
	}
	return dstaddr;
}

void *memset(void *dst, int c, size_t n) {
	void *dstaddr = dst;
	void *max = dst + n;
	u_char byte = c & 0xff;
	uint32_t word = byte | byte << 8 | byte << 16 | byte << 24;

	while (((u_long)dst & 3) && dst < max) {
		*(u_char *)dst++ = byte;
	}

	// fill machine words while possible
	while (dst + 4 <= max) {
		*(uint32_t *)dst = word;
		dst += 4;
	}

	// finish the remaining 0-3 bytes
	while (dst < max) {
		*(u_char *)dst++ = byte;
	}
	return dstaddr;
}

size_t strlen(const char *s) {
	int n;

	for (n = 0; *s; s++) {
		n++;
	}

	return n;
}

char *strcpy(char *dst, const char *src) {
	char *ret = dst;

	while ((*dst++ = *src++) != 0) {
	}

	return ret;
}

const char *strchr(const char *s, int c) {
	for (; *s; s++) {
		if (*s == c) {
			return s;
		}
	}
	return 0;
}

int strContain(const char *parent, const char *child, char *waitToComp) {
	while (*parent == *child) {
		parent++;
		child++;
	}
	if (*child == 0) {
		strcpy(waitToComp, parent);
		return 0;
	}
	return -1;
}

int strcmp(const char *p, const char *q) {
	while (*p && *p == *q) {
		p++, q++;
	}

	if ((u_int)*p < (u_int)*q) {
		return -1;
	}

	if ((u_int)*p > (u_int)*q) {
		return 1;
	}

	return 0;
}

char *strcat(char *dst, const char *src) {
	char *ret = dst;
	while (*dst != 0){
		dst++;
	}
	
	while ((*dst++ = *src++) != 0) {
	}
	return ret;
}

char *catPath(char *pre, const char *path) {
	char *ret = pre;
	while (*pre != 0){
		pre++;
	}
	if (*(pre - 1) != '/' && *path != '/') {
		*(pre++) = '/';
	}
	while ((*pre++ = *path++) != 0) {
	}
	return ret;
}

char *toParentPath(char *pathName) {
	if (strcmp(pathName, "/") == 0) {
		return pathName;
	}
	int len = strlen(pathName);
	while(len--) {
		if (pathName[len] == '/') {
			pathName[len] = 0;
			break;
		}
		pathName[len] = 0;
	}
	if (pathName[0] == 0) {
		pathName[0] = '/';
	}
	return pathName;
}

int splitPath(char *firDir, char* nextPath, const char *pathName) {
	if (pathName[0] == '/') {
		return -1;
	}
	int i = 0;
	while (pathName[i] != '/' && pathName[i] != 0) {
		if (i >= MAXNAMELEN) {
			return -1;
		}
		firDir[i] = pathName[i];
		i++;
	}
	firDir[i] = 0;
	if (pathName[i] == 0) {
		return 0;
	}

	int nexti = 0;
	i++;
	while (pathName[i] != 0) {
		nextPath[nexti++] = pathName[i++];
	}
	nextPath[nexti] = 0;
	return 0;
}

int strToUint(char *str) {
	int r = 0;
	int i;
	for (i = 0; str[i] != 0; i++) {
		if ('0' <= str[i] && str[i] <= '9') {
			r = (str[i] - '0') + r * 10;
		} else {
			return -1;
		}
	}
	return r;
}