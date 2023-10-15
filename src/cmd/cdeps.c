#include "cdeps.h"

int execute(int argc, char *argv[]) {
    if (argc < 2) {
        return usage();
    }

    if (strcmp("version", argv[1]) == 0) {
        return version();
    } else if (strcmp("install", argv[1]) == 0) {
        if (argc == 2) {
            return install(NULL);
        } else if (argc == 3) {
            return install(argv[2]);
        } else {
            return usage();
        }
    } else if (strcmp("clean", argv[1]) == 0) {
        return clean();
    } else if (strcmp("list", argv[1]) == 0) {
        return list();
    } else {
        return usage();
    }
}