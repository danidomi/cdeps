#include "usage.h"

int usage() {
    printf("cdeps is a tool for managing C source code.\n"
           "\n"
           "Usage:\n"
           "\n"
           "\tcdeps <cmd> [arguments]\n"
           "\n"
           "The commands are:\n"
           "\n"
           "\tclean       remove object files and cached files\n"
           "\tinstall     compile and install dependencies\n"
           "\tlist        list dependencies\n"
           "\tversion     print cdeps version\n");
    return 0;
}