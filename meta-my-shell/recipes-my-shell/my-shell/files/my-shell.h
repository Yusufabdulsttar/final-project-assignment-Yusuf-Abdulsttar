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
#include <signal.h>

/* section : Micro Declaration  */

#define MAX_COMMAND_LENGTH 1024
#define MAX_ARGS 10

#define MAX_BG_PROCESSES 10

#define Exit 1
#define Success 2

// for prompt
#define PROMPT_SIZE 256
#define BLUE "\033[1;34m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"

/* section : Micro Function Declaration  */

/* section : Data Type Declaration  */

char command[MAX_COMMAND_LENGTH]; //array to store the commands
char *args[MAX_ARGS]; // Array to hold command arguments

// for execute pipes and background process
int pipe_pos = -1;
int is_background = 0;

char *token=NULL; //token for parsing
int number_of_token = 0; //number of token
	
pid_t bg_processes[MAX_BG_PROCESSES]; // Array to store background process IDs
pid_t foreground_pid = -1; // keep track of the foreground process PID
int num_bg_processes = 0; // Counter for the number of background processes
int bring_to_fg = 0;
	
/* section : Function Declaration  */

int read_command(char *command,int max_length);
int prompt(void);
int parse_command(char *command,char** args,char* token);
int built_in_command(char** args);
void execute_background(char** args);
void bring_to_foreground(char** args,int* position);
void execute_with_pipe(char **args, int pipe_pos);
void execute_command(char** args);
void signal_handler(int signal);

#endif	/* MY_SHELL_H */
