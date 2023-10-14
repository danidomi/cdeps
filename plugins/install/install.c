#include "install.h"

const char * downloadLatestURL = "https://%s/releases/latest/download/%s.zip";
const char * downloadVersionURL = "https://%s/releases/download/%s%s.zip";

char * get_download_latest_URL(const char *repoURL, const char *repoName) {
    char *url = (char *)malloc(256);
    sprintf(url, downloadLatestURL, repoURL, repoName);
    return url;
}

char * get_download_version_URL(const char *repoURL, const char *version,const char *repoName) {
    char *url = (char *)malloc(256);
    sprintf(url, downloadVersionURL, repoURL, version, repoName);
    return url;
}

void download(char *repoURL, char *version) {
    char command[256];
    char * repoName = strrchr(repoURL, '/');
    char * url = NULL;
    if (strcmp(version, LATEST_VERSION) == 0) {
        url = get_download_latest_URL(repoURL, repoName);
    }else {
        url = get_download_version_URL(repoURL, version, repoName);
    }
    snprintf(command, sizeof(command), "curl -o %s%s.zip -sLJO %s", FOLDER, repoName, url);
    system(command);
}

void unzip(char *repoURL) {
    char command[256];
    char * repoName = strrchr(repoURL, '/');
    snprintf(command, sizeof(command), "unzip -q %s%s.zip -d %s && rm %s%s.zip", FOLDER, repoName, FOLDER, FOLDER, repoName);
    system(command);
}

int handleSingleInstall(const char *dependency) {
    // Check if the package includes a '@' character
    if (strchr(dependency, '@') == NULL) {
        printf("cdeps: 'cdeps install <dependency>@<version>' requires a version when not provided a '%s' file.\n", CDEPS_FILE);
        return 1;
    }
    char *repoURL = NULL;
    char *version = NULL;
    char *string = strdup(dependency);

    repoURL = strsep(&string, "@");
    version = strsep(&string, "\0");  // Change "\n" to "\0"
    download(repoURL, version);
    unzip(repoURL);

    // Free the allocated memory for 'string'
    free(string);

    return 0;
}

int handleFileInstall() {
    FILE *file = fopen(CDEPS_FILE, "r");

    if (!file) {
        printf("Error opening %s file", CDEPS_FILE);
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
}

int createFolder() {


    // Use the stat function to check if the folder exists
    struct stat st;
    if (stat(FOLDER, &st) != 0) {
        // The folder doesn't exist, so create it
        return mkdir(FOLDER, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
    }

    return 0;
}


int install(const char *dependency) {
    int success = createFolder();
    if (success != 0) {
        printf("Unable to create 'deps' folder");
        return success;
    }

    if(dependency == NULL) {
        return handleFileInstall();
    }

    return handleSingleInstall(dependency);
}


