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
		number_of_token = parse_command(command,args,token);

		//search for pipes
		int pipe_pos = -1;
		for (int j = 0; j < number_of_token; j++) {
			if (strcmp(args[j], "|") == 0) {
				pipe_pos = j;
				break;
			}
		}

		if (pipe_pos != -1) {
			// execute commands with pipes
			execute_with_pipe(args, pipe_pos);
		} else {
			// execute commands without pipes
			if (execute_command(args) == Exit){
			 		break;
				}
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
    
    return i;
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
	
	
// function to handle a single pipe
void execute_with_pipe(char **args, int pipe_pos) {

    int pipefd[2];
    pid_t pid1, pid2;

    if (pipe(pipefd) == -1) {
        perror("pipe");
    }

    pid1 = fork();
    if (pid1 == 0) {
        // First child: executes the command before the pipe
        close(pipefd[0]); // Close unused read end
        dup2(pipefd[1], STDOUT_FILENO); // Write end becomes stdout
        close(pipefd[1]);

        args[pipe_pos] = NULL; // Null terminate the first command
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE); // Exit child process
        }
    }

    pid2 = fork();
    if (pid2 == 0) {
        // Second child: executes the command after the pipe
        close(pipefd[1]); // Close unused write end
        dup2(pipefd[0], STDIN_FILENO); // Read end becomes stdin
        close(pipefd[0]);

        if (execvp(args[pipe_pos + 1], &args[pipe_pos + 1]) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Close pipe fds in parent
    close(pipefd[0]);
    close(pipefd[1]);

    // Wait for children to exit
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

