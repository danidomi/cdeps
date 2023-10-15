#include "clean.h"

int clean() {
    char command[256];

    // Check if the directory exists
    if (access(FOLDER, F_OK) == 0) {

        // Delete the directory and its contents
        sprintf(command, "rm -r %s", FOLDER);
        return system(command);
    }

    return 0;
}