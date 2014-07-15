startCode
=========

Application to setup start-up code files to facilitate programming

Setup guide:

  1. To install:
      $make
      $make install
    
      This compiles code, places the binary in /usr/bin and sets up initial templates in ~/.startCode/templates.
      NOTE: requires super-user privileges for make install. 

  2. To clean up:
      $make clean
      
  3. To uninstall:
      $make uninstall

      NOTE: requires super-user privileges.
      

HELP :

  startCode is an application to create or fill startup content for
  programming code files. 
 
  Author: Kushal Prasad
  Date: 12 July 2014
 
Usage: 
       startCode options filename destdir
      
       This command places the custom startup file (with filename) 
       in the specified destination directory.
 
   options
       -cc : C programming .c (code) file
       -ch : C programming .h (header) file
       -m  : Makefile
       -h  : Help or manual page display
 
   filename
       If file already exists at the location (specified by destdir),
       content is inserted at the end, otherwise a 
       new file with the pre-filled content is created.
 
   destdir
       Directory in which resultant file should be placed. This directory can be
       relative to current working directory (.) or an absolute path (starting with '/' or '~')
 
  


