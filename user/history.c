#include <lib.h>
#define MAX_HIS_NUM 1024
#define _OP_ERR_                                                                                   \
	"history: missing operand \n\
Try 'history --help' for more information. \n"
#define _HELP_                                                                                     \
	"Usage: history [-l] or history -n N\n\
Display the history cmd list. \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -l, --line        display the line num \n\
  -n, --num         The parameter N is required. Display the the most recent N items \n\
      --help        display this help and exit \n"

char *history_buf[MAX_HIS_NUM];
int flag[256];

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		printf(_HELP_);
		return 0;
	}
	ARGBEGIN {
	case 'l':
	case 'n':
		flag[(u_char)ARGC()]++;
		break;
	}
	ARGEND

	int len = history_get_all(history_buf);
	int i = 0;
	if (flag['n']) {
		if (argv[0][0] == 0) {
			printf(_OP_ERR_);
			return -1;
		}
		int r = strToUint(argv[0]);
		if (r < 0) {
			printf(_OP_ERR_);
			return -1;
		} else {
			i = len - r;
			if (i < 0) {
				debugf("There are no %d items at the moment\n", r);
				return -1;
			}
		}
	} else {
		i = 0;
	}

	printf("total history cmd num is %d\n", len);

	if (flag['l']) {
		for (; i < len; i++) {
			printf("%4d : %s\n", i + 1, history_buf[i]);
		}
	} else {
		for (; i < len; i++) {
			printf("%s\n", history_buf[i]);
		}
	}

	return 0;
}