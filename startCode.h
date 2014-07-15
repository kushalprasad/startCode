#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>

#define ARGUMENT_MAX 4
#define ARGUMENT_MIN 4

/***************** SERVICE METHODS DECLARATIONS ***************/

// Verify correctness of arguments and deliver mode of operation
int chk_args(int argc, char** argv);

// Fill in new file buffer based on mode
int setup_file(int mode, char** argv);

// Print custom message and print the manual page
void manprint(char* msg);
