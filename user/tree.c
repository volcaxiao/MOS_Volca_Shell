#include <lib.h>
#define _OP_ERR_                                                                                   \
	"tree: missing operand \n\
Try 'tree --help' for more information. \n"

#define _HELP_                                                                                     \
	"Usage: tree [-adfs] [-L level] \n\
Print all files in the specified directory in a tree. \n\
By default, Print the current directory \n\
If there are other parameters, ignore them \n\
\n\
Mandatory arguments to long options are mandatory for short options too. \n\
  -a            All files are listed. \n\
  -d            List directories only. \n\
  -f            Print the full path prefix for each file. \n\
  -s            Print the size in bytes of each file. \n\
  -L level      Descend only level directories deep. \n\
     --help     display this help and exit \n"

#define MAXLEVEL 1024

int flag[256];
int maxLevel;

void identPrint(const char *path, struct File file,  const char *ident) {
    char pathName[MAXPATHLEN];
    if (flag['f']) {
        strcpy(pathName, path);
        catPath(pathName, file.f_name);
    } else {
        strcpy(pathName, file.f_name);
    }
    if (flag['s']) {
        if (file.f_type == FTYPE_DIR) {
            printf("%s|-\033[32;1m[%8dB]\033[0m\033[34;1m%s\033[0m\n", ident, file.f_size, pathName);
        } else {
            printf("%s|-\033[32;1m[%8dB]\033[0m%s\n", ident, file.f_size, pathName);
        }
    } else {
        if (file.f_type == FTYPE_DIR) {
            printf("%s|-\033[34;1m%s\033[0m\n", ident, pathName);
        } else {
            printf("%s|-%s\n", ident, pathName);
        }
    }
}

int print_tree(int dep, const char *path, const char *ident) {
    if (dep > maxLevel)  {
        return -1;
    }
	int r;
	int fd = open(path, O_RDONLY);
	if (fd < 0) {
		debugf(_OP_ERR_);
	}
	struct File file = ((struct Filefd *)num2fd(fd))->f_file;
	if (file.f_type == FTYPE_DIR) {
		int n;
		struct File son;
		while ((n = readn(fd, &son, sizeof son)) == sizeof son) {
			if (son.f_name[0] == 0 || (son.f_name[0] == '.' && !flag['a'])) {
				continue;
			}
			if (son.f_type == FTYPE_DIR) {
                identPrint(path, son, ident);
				char sonPath[MAXPATHLEN];
				strcpy(sonPath, path);
				catPath(sonPath, son.f_name);
				char sonIndent[MAXPATHLEN];
				strcpy(sonIndent, ident);
                if (flag['s']) {
                    strcat(sonIndent, "|     ");
                } else {
                    strcat(sonIndent, "|  ");
                }
				print_tree(dep + 1, sonPath, sonIndent);
			} else if (son.f_type == FTYPE_REG && !flag['d']) {
				identPrint(path, son, ident);
			}
		}
	} else {
		return -1;
	}
	return 0;
}

int main(int argc, char **argv) {
	if (strcmp(argv[1], "--help") == 0) {
		debugf(_HELP_);
		return 0;
	}
    ARGBEGIN {
	    case 'a':
	    case 'd':
        case 'f':
        case 's':
        case 'L':
		    flag[(u_char)ARGC()]++;
		    break;
	}
	ARGEND

    int beginArg = 0;
    if (flag['L']) {
        maxLevel = strToUint(argv[0]);
        if (maxLevel < 0) {
            debugf("Level cannot be minus num!\n");
            debugf(_OP_ERR_);
            return -1;
        }
        beginArg = 1;
    } else {
        maxLevel = MAXLEVEL;
    }

	char treePath[MAXPATHLEN];
	if (argc == beginArg) {
		syscall_get_env_path(0, treePath);
	} else if (argc == beginArg + 1){
		strcpy(treePath, argv[beginArg]);
	} else {
        debugf(_OP_ERR_);
    }
	printf("->\033[34;1m%s\033[0m\n", treePath);
	print_tree(1, treePath, "");
	return 0;
}