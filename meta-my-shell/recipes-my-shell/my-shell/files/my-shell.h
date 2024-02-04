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
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

/* section : Micro Declaration  */

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 10
#define Exit 1

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */

char command[MAX_COMMAND_LENGTH]; //array to store the commands
char *args[MAX_ARGS]; // Array to hold command arguments
char *token=NULL; //token for parsing
int number_of_token = 0;
	
/* section : Function Declaration  */

int read_command(char *command,int max_length);
int parse_command(char *command,char** args,char* token);
void execute_with_pipe(char **args, int pipe_pos);
int execute_command(char** args);

#endif	/* MY_SHELL_H */
