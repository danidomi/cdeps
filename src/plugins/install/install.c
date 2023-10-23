#include "install.h"

const char *downloadLatestURL = "https://%s/releases/latest/download/%s_%s.zip";
const char *downloadVersionURL = "https://%s/releases/download/%s/%s_%s.zip";

char *get_download_latest_URL(const struct utsname systemInfo, const char *repoURL, const char *repoName) {
    char *url = (char *) malloc(256);
    sprintf(url, downloadLatestURL, repoURL, systemInfo.sysname, systemInfo.machine);
    return url;
}

char *get_download_version_URL(const struct utsname systemInfo, const char *repoURL, const char *version) {
    char *url = (char *) malloc(256);
    sprintf(url, downloadVersionURL, repoURL, version, systemInfo.sysname, systemInfo.machine);
    return url;
}

int download(const struct utsname systemInfo, char *repoURL, char *version) {
    char command[256];
    char *repoName = strrchr(repoURL, '/');
    char *url = NULL;

    if (strcmp(version, LATEST_VERSION) == 0) {
        url = get_download_latest_URL(systemInfo, repoURL, repoName);
    } else {
        url = get_download_version_URL(systemInfo, repoURL, version);
    }
    snprintf(command, sizeof(command), "curl -o %s/%s_%s.zip -sLJO %s", FOLDER, systemInfo.sysname, systemInfo.machine,
             url);

    return system(command);
}

int unzip(const struct utsname systemInfo, char *repoURL) {
    char command[256];
    char *repoName = strrchr(repoURL, '/');
    snprintf(command, sizeof(command), "unzip -q %s/%s_%s.zip -d %s%s && rm %s/%s_%s.zip", FOLDER, systemInfo.sysname,
             systemInfo.machine, FOLDER, repoName, FOLDER,
             systemInfo.sysname, systemInfo.machine);

    return system(command);
}

int handleSingleInstall(const char *dependency) {
    // Check if the package includes a '@' character
    if (strchr(dependency, '@') == NULL) {
        printf("%s: '%s install <dependency>@<version>' requires a version when not provided a '%s' file.\n",
               CMD, CMD, CDEPS_FILE);
        return 1;
    }
    char *repoURL = NULL;
    char *version = NULL;
    char *string = strdup(dependency);

    struct utsname systemInfo;

    if (uname(&systemInfo) == -1) {
        printf("unable to uname");
        return 1;
    }

    repoURL = strsep(&string, "@");
    version = strsep(&string, "\0");  // Change "\n" to "\0"
    if (download(systemInfo, repoURL, version) != 0) {
        printf("Unable to download dependency");
        return 1;
    }

    if (unzip(systemInfo, repoURL) != 0) {
        printf("Unable to unzip dependency");
        return 1;
    }

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

    struct utsname systemInfo;

    if (uname(&systemInfo) == -1) {
        printf("unable to uname");
        return 1;
    }

    while (fgets(line, sizeof(line), file)) {
        // Check if the line contains at least one space
        if (strchr(line, ' ') == NULL) {
            printf("Invalid dependency: %s\n", line);
            continue;
        }

        if (line[0] == '#' || strlen(line) == 0) {
            continue;  // Skip comments or empty lines
        }

        char *string = strdup(line);
        repoURL = strsep(&string, " \n");
        version = strsep(&string, " \n");
        if (download(systemInfo, repoURL, version) != 0) {
            printf("Unable to download dependency: %s\n", line);
            return 1;
        }

        if (unzip(systemInfo, repoURL) != 0) {
            printf("Unable to unzip dependency");
            return 1;
        };
    }

    return fclose(file);
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

    if (dependency == NULL) {
        return handleFileInstall();
    }

    return handleSingleInstall(dependency);
}
