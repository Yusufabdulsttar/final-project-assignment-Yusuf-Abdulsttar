/* 
 * File:   my-shell.c
 * Author: Yusuf Abdulsttar
 *
 * Created on jan 30, 2024, 1:48 PM
 */
#include "my-shell.h"

int main() {
	
    while (1) {
    
        printf("myshell> ");  // Print the prompt

		//read the command
		if(read_command(command,MAX_COMMAND_LENGTH) == Exit){
			break;
		}
		
		//parse the command
		if(parse_command(command,args,token) == Exit){
			break;
		}
		
		//execute the command
		if(execute_command(args) == Exit){
			break;
		}
       
      }

    return 0;
}

int read_command(char *command,int max_length){
    if (!fgets(command, max_length, stdin)) {
        // Handle error or end of Transmission (Ctrl+D)
        printf("\nExiting...\n");
        return Exit;
    }
    return 0;
}

int parse_command(char *command,char** args,char* token){

    // Parse the input into arguments
	int i= 0;
	
    token = strtok(command, " \n"); // Split input by spaces and \n if there is one command
    
    while (token != NULL && i < MAX_ARGS - 1) {
        args[i++] = token;
        token = strtok(NULL, " \n");
    }
    
    args[i] = NULL; // Null-terminate the argument list
    
    return 0;
}

int execute_command(char** args){

    // Execute the command
    int status= 0;
    
    pid_t pid = fork(); // Create a new process
    
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("my-shell"); // Print error if execvp fails
            return Exit;
        }
        exit(EXIT_FAILURE); // Exit child process
        
    } else if (pid < 0) {
        // Forking error
        perror("fork");
        return Exit;
        
    } else {
        // Parent process waits for child to complete
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 0;
}
	
