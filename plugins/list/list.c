#include "list.h"

int list() {
    // Open the "deps" directory
    DIR* dir = opendir(FOLDER);

    if (dir == NULL) {
        return 1;
    }

    struct dirent* entry;
    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." directory entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Check if the entry is a directory
        if (entry->d_type == DT_DIR) {
            printf("%s\n", entry->d_name);
        }
    }

    closedir(dir);

    return 0;
}