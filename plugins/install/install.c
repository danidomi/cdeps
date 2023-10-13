#include "install.h"


void downloadObjectFile(char *repoURL, char *version) {
    char command[256];
    char * downloadDir = "deps";
    snprintf(command, sizeof(command), "curl -o %s/%s.o -sLJO https://%s/archive/%s/%s ", downloadDir, version, repoURL, version, repoURL);
    system(command);
}


int install() {
    FILE *file = fopen("c.deps", "r");

    if (!file) {
        perror("Error opening c.deps file");
        return 1;
    }

    char line[MAX_LINE_LENGTH];
    char *repoURL = NULL;
    char *version = NULL;

    while (fgets(line, sizeof(line), file)) {
        if (line[0] == '#' && strlen(line) == 0) {
            continue;  // Skip comments
        }

        char *string = strdup(line);
        repoURL = strsep(&string, " \n");
        version = strsep(&string, " \n");
        downloadObjectFile(repoURL, version);
    }

    fclose(file);
    return 0;
}


