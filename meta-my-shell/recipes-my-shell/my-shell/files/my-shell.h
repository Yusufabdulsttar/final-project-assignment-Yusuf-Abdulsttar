/* 
 * File:   my-shell.h
 * Author: Yusuf Abdulsttar
 *
 * Created on jan 30, 2024, 1:48 PM
 */

#ifndef MY_SHELL_H
#define	MY_SHELL_H

/* section : Includes   */

#include <stdio.h>

/* section : Micro Declaration  */

#define MAX_COMMAND_LENGTH 1024
#define Exit 1
/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */

/* section : Function Declaration  */

int read_command(char *command,int max_length);

#endif	/* MY_SHELL_H */
