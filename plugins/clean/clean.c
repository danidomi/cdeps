#include "clean.h"

int clean() {
    char command[256];

    // Check if the directory exists
    if (access(FOLDER, F_OK) == 0) {
        printf("Directory '%s' exists. Deleting...\n", FOLDER);

        // Delete the directory and its contents
        sprintf(command, "rm -r %s", FOLDER);
        if (system(command) == 0) {
            printf("Directory deleted successfully.\n");
        } else {
            perror("Error deleting directory");
        }
    } else {
        printf("Directory '%s' does not exist.\n", FOLDER);
    }

    return 0;
}