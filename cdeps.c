#include "cdeps.h"

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return usage();
    }

    if (strcmp("version", argv[1]) == 0) {
        return version();
    } else if (strcmp("install", argv[1]) == 0) {
        return install();
    } else if (strcmp("clean", argv[1]) == 0) {
        return clean();
    }
}