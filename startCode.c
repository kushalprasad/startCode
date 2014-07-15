/*
 * startCode is an application to create or fill startup content for
 * programming code files. 
 *
 * Author: Kushal Prasad
 * Date: 12 July 2014
 *
 * Usage:
 *  
 *  startCode [options] filename dest_path
 *      This command places the custom startup file (with filename) 
 *      in the specified directory.
 *
 *  [options]
 *      -cc : C programming .c (code) file
 *      -ch : C programming .h (header) file
 *      -m  : Makefile
 *      -h  : Help or manual page display
 *
 *  filename
 *      If file already exists at the location (specified by destdir),
 *      content is inserted at the beginning of the file otherwise a 
 *      new file with the content is created.
 *
 *  destdir
 *      Directory in which resultant file should be placed. This directory can be
 *      relative to current working directory (.) or an absolute path (starting with '/' or '~')
 *
 * 
 * 
 *
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include "startCode.h"

#define ARGUMENT_MIN 4
#define ARGUMENT_MAX 4


char file_path[256];

/******************** MAIN METHOD ******************************/
int main (int argc, char** argv)
{
    struct passwd *pw = getpwuid(getuid());
    strcpy(file_path, pw->pw_dir);
    printf("\n Home dir : %s\n", file_path); 
    // Send arguments to get verified
    // Get back the mode of operation
    int mode = chk_args(argc, argv);

    // Setup file contents depending on mode
    int done = setup_file(mode, argv);
    if(done != 0)
    {
        manprint("Error occured during setting up the file!");
        exit(0);
    }

    return 0;
}
///////////////////////////////////////////////////////////////////


/***************** SERVICE METHOD DEFINITIONS *******************/
int chk_args(int argc, char** argv)
{
    int ret = 0;
    
    // Check if enough arguments were provided
    if(argc < 2)
    {
        // Invalid
        manprint("Incorrect usage! ");
        exit(0);
    }

    int def = 0;
    char temp[256];
    strcpy(temp, file_path);
    FILE *optPtr = fopen(strcat(temp,"/.startCode/templates/options.cfg"),"r");
    if(optPtr == NULL)
    {
        printf("\n Could not find ~/.startCode/options.cfg! Using defaults... \n");
        printf(" Use startCode --help or startCode -h for default usage options\n");
        def = 1;
    }

//    if(def == 1) /* Default mode */
    {
        // check mode and validity of [option]
        if(argc == 4)
        {
            if(!strcmp(argv[1],"-cc"))
            {
                ret = 1;
            }
            else if(!strcmp(argv[1],"-ch"))
            {
                ret = 2;
            }
            else if(!strcmp(argv[1],"-m"))
            {
                ret = 3;
            }
        }
        else if(!strcmp(argv[1],"-h"))
        {
            // Display HELP and exit
            manprint("");
            exit(0);
        }
        else
        {
            // Invalid
            manprint("Incorrect usage!");
            exit(0);
        }
    }
    
    // Check filename validity
    if(strlen(argv[2]) <= 0 || strlen(argv[2]) > 30)
    {
        manprint("Invalid filename - either absent or too long");
        exit(0);
    }

    // Check destination path validity
    if(strlen(argv[3]) < 256)
    {

        DIR* dir = opendir(argv[3]);
        if (dir)
        {
            /* Directory exists. */
            closedir(dir);
        }
        else
        {
            manprint("Directory does not exist or access not permitted!");
            exit(0);
            /* opendir() failed for some other reason. */
        }
    }
    else
    {
        manprint("Destination path is too long");
        exit(0);
    }

    return ret;
}

int setup_file(int mode, char** argv)
{
    int ret = -1;

    // Fill current working directory in 'filepath' variable
    // Finalize validity of directory and construct full filename
    // fully qualified file path cannot be more than PATH_MAX characters
    char filepath[PATH_MAX];
   
    if(argv[3][0] == '/' || argv[3][0] == '~')
    {
        // Use as is for CWD
        strncpy(filepath, argv[3], strlen(argv[3]));
    }
    else if(argv[3][0] == '.')
    {
        if((!strcmp(argv[3],".")) || (!strcmp(argv[3],"./")))
        {
            char *path_var;
            path_var = getcwd(filepath, PATH_MAX);        
            if(path_var != filepath)
            {
                manprint("Could not retrieve CWD");
                exit(0);
            }
            // Suffix the trailing '/'
        }
        else if(argv[3][0] == '.' && argv[3][1] == '/')
        {
            char *path_var;
            // Get current working directory
            path_var = getcwd(filepath, PATH_MAX);        
            if(path_var != filepath)
            {
                manprint("Could not retrieve CWD");
                exit(0);
            }
            // Concatenate the specified location
            strcat(filepath,argv[3]+1);
        }
    }
    else
    {
        manprint("Invalid destination pathname!");
        exit(0);
    }

    // Suffix a trailing '/' if required
    if(filepath[strlen(filepath)-1]!='/')
    {
        strcat(filepath, "/");
    }

    strcat(filepath, argv[2]);

    // printf("\n FILENAME : %s \n\n", filepath);
    // Initialize file descriptor
    // a+ - Open for reading and appending (writing at end of file).  The
    // file is created if it does not exist.  The initial file
    // position for reading is at the beginning of the file, but
    // output is always appended to the end of the file.
    FILE *destPtr = fopen(filepath , "a+");
    if (destPtr == NULL)
    {
        printf("\n ERROR: Could not open destination file... Check access rights!\n Exiting...\n");
        exit(0);
    }

    // Fill file with content from defaults based on mode
    // Set source pointer.
    char temp[256];
    strcpy(temp,file_path);
    FILE *srcPtr;
    switch(mode)
    {
        // C program code case
        case 1:
                srcPtr = fopen(strcat(temp,"/.startCode/templates/ccode.template"),"r");
                printf("\n Path : %s\n",temp);
                if( srcPtr == NULL )
                {
                    printf("\n ERROR: Could not access template file ~/.codeTemplate/templates/ccode.template\n");
                    exit(0);
                }
                break;
        // C program header case
        case 2:
                srcPtr = fopen(strcat(temp,"/.startCode/templates/cheader.template"),"r");
                if( srcPtr == NULL )
                {
                    printf("\n ERROR: Could not access template file ~/.codeTemplate/templates/cheader.template\n");
                    exit(0);
                }
                break;
        // Makefile case
        case 3:
                srcPtr = fopen(strcat(temp,"/.startCode/templates/makefile.template"),"r");
                if( srcPtr == NULL )
                {
                    printf("\n ERROR: Could not access template file ~/.codeTemplate/templates/makefile.template\n");
                    exit(0);
                }
                break;
        // Invalid case
        default:
                manprint("mode was not setup correctly! Fault in code!");
                exit(0);
    }

    // Fetch line by line and fill into destination
    char line [256]; 
    while ( fgets ( line, sizeof(line), srcPtr ) != NULL ) 
    {
        fputs (line, destPtr); 
    }
    
    // Clean up resources
    fclose(srcPtr);
    fclose(destPtr);
    
    return 0;
}

void manprint(char* msg)
{
    if(strcmp(msg,""))
        printf("\n ERROR: %s", msg);

    printf("\n\n");
    printf("\t codeTemplate is an application to create or fill startup content for \n\tprogramming code files.");
    printf("\n\n Usage:");
    printf("\n\n");
    printf("\t startCode [options] filename dest_path");
    printf("\n\t\t This command places the custom startup file (with filename) ");
    printf("\n\t\t in the specified directory. Extension is appended automatically.");
    printf("\n\n\t [options]");
    printf("\n\t\t -cc : C programming .c (code) file");
    printf("\n\t\t -ch : C programming .h (header) file");
    printf("\n\t\t -m  : Makefile");
    printf("\n\t\t -h  : Help or manual page display");
    printf("\n\n\t filename");
    printf("\n\t\t If file already exists at the location (specified by destdir),");
    printf("\n\t\t content is inserted at the beginning of the file otherwise a ");
    printf("\n\t\t new file with the content is created. Note: Extension is automatically applied.");
    printf("\n\n\t destdir");
    printf("\n\t\t Directory in which resultant file should be placed. This directory can be");
    printf("\n\t\t relative to current working directory (.) or an absolute path (starting with '/' or '~')");
    printf("\n");
    return;
}
