#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "../../common/common.h"

#define MAX_LINE_LENGTH 256
#define LATEST_VERSION "latest"
#define CDEPS_FILE "c.deps"

int install(const char *dependency);