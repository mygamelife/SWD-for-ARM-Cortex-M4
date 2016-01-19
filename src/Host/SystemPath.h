#ifndef SystemPath_H
#define SystemPath_H

#include <stdio.h>  /* defines FILENAME_MAX */
#include <string.h>
#ifdef WINDOWS
    #include <direct.h>
    #define GetCurrentDir _getcwd
#else
    #include <unistd.h>
    #define GetCurrentDir getcwd
 #endif

#define FP_INSIDE_CWD     "./target/FlashProgrammer.elf"
#define FP_OUTSIDE_CWD    "./build/release/target/FlashProgrammer.elf"

int isCurrentDirectory(char *cwd);
char *getFpPath(char *cwd);
char *getCurrentDirectory(void);

#endif // SystemPath_H
