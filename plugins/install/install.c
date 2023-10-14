#include "install.h"

void download(char *repoURL, char *version) {
    char command[256];
    char * downloadDir = "deps";
    char * repoName = strrchr(repoURL, '/');
    snprintf(command, sizeof(command), "curl -o %s/%s.zip -sLJO https://%s/releases/download/%s%s.zip", downloadDir, repoName, repoURL, version, repoName);
    system(command);

}

void unzip(char *repoURL) {
    char command[256];
    char * downloadDir = "deps";
    char * repoName = strrchr(repoURL, '/');
    snprintf(command, sizeof(command), "unzip -q %s%s.zip -d %s && rm %s%s.zip", downloadDir, repoName, downloadDir, downloadDir, repoName);
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
        download(repoURL, version);
        unzip(repoURL);
    }

    fclose(file);
    return 0;
}
