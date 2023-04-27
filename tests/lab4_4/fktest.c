#include <lib.h>

int main() {
	int a = 0;
	int id = 0;
	debugf("fktest on ostest!\n");
	if ((id = fork()) == 0) {
		debugf("child\n");
		return 0;
	}
	a++;
	int iii;
	for (iii = 0; iii < 50; iii++) {
		debugf("@this is father: a:%d\n", a);
		syscall_yield();
	}
	debugf("@father exit\n");
	return 0;
}
