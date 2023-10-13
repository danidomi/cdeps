#include "clean.h"

int clean() {
    const char* directoryName = "deps"; // Change this to your directory name
    char command[256];

    // Check if the directory exists
    if (access(directoryName, F_OK) == 0) {
        printf("Directory '%s' exists. Deleting...\n", directoryName);

        // Delete the directory and its contents
        sprintf(command, "rm -r %s", directoryName);
        if (system(command) == 0) {
            printf("Directory deleted successfully.\n");
        } else {
            perror("Error deleting directory");
        }
    } else {
        printf("Directory '%s' does not exist.\n", directoryName);
    }

    return 0;
}