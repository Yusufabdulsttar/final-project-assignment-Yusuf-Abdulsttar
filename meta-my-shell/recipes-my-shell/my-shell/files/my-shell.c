/* 
 * File:   my-shell.c
 * Author: Yusuf Abdulsttar
 *
 * Created on jan 30, 2024, 1:48 PM
 */
#include "my-shell.h"

int main() {

    while (1) {
	    // Register signal handler for both SIGINT and SIGTSTP
    	signal(SIGINT, signal_handler); // Register SIGINT handler
   		signal(SIGTSTP, signal_handler); // Register SIGTSTP handler
   		
		//Print the prompt
		if(prompt() == Exit){
			printf("Error in prompt");
		}	

		//read the command
		if(read_command(command,MAX_COMMAND_LENGTH) == Exit){
			printf("Error in read");
		}
		
		//parse the command
		number_of_token = parse_command(command,args,token);
		if (number_of_token == 0){
			continue; // No command entered
		}
		
		//built-in command
		if (built_in_command(args) == Success){
			continue; // Process next command
		}
		else if (built_in_command(args) == Exit){
			break; // Exit the shell
		}
		
		// search for pipes and background process
		pipe_pos = -1;
		bring_to_fg = -1;
		is_background = 0;
		for (int j = 0; j < number_of_token; j++) {
			//search for fg to bring background process
			if (strcmp(args[j], "fg") == 0) { 	
				bring_to_fg = j;    
				break;
			}
			//search for background process
			else if (strcmp(args[j], "&") == 0) { 
				is_background = 1;
				args[j] = '\0'; // Remove (&) from command
				break;
			}
			//search for pipes
			else if (strcmp(args[j], "|") == 0) { 
				pipe_pos = j;
				break;
			}
		}
		
		// execute commands
		if (bring_to_fg != -1){ 
			// bring the background process to the foreground
	        bring_to_foreground(args,&bring_to_fg);     
			continue; 
		}
		else if (is_background){
			// execute command in background
			execute_background(args);
		}
		else if (pipe_pos != -1) {
			// execute commands with pipes
			execute_with_pipe(args, pipe_pos);
		} else {
			// execute commands normally
			execute_command(args);
		}
			
     }

    return 0;
}

int prompt(void){
    // Print the prompt
    
	char *cwd = (char *)malloc(PROMPT_SIZE);
	if (cwd == NULL){
		printf("Allocation failed");
		return Exit;
	}

    printf("%sMy-shell:%s", BLUE, RESET);
    // Get the current working directory
    if (getcwd(cwd, PROMPT_SIZE) == NULL) {
        perror("getcwd");
        return Exit;
    }
	printf("%s%s>%s ", YELLOW, cwd, RESET);
	free(cwd);
	
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

// Check for built-in commands 
int built_in_command(char** args){

    if (strcmp(args[0], "cd") == 0) {
        // Built-in command: cd
        if (args[1] == NULL) {
            fprintf(stderr, "Expected argument to \"cd\"\n");
            return Success;
            
        } else {
            if (chdir(args[1]) != 0) {
                perror("My-shell");
            }
           return Success;
        }
        
    } else if (strcmp(args[0], "exit") == 0) {
        // Built-in command: exit
        printf("\nExiting...\n");
       return Exit; 
    }
	return 0;
}

// Function to execute a command in the background
void execute_background(char** args) {
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("execvp");
            exit(EXIT_FAILURE);  // Exit child process
        }
    } else if (pid < 0) {
        // Error forking
        perror("fork");
    } else {
        // Parent process
        // Print background process information
        printf("Background process with PID %d started.\n", pid);
        
        // Add background process to list
        if (num_bg_processes < MAX_BG_PROCESSES) {
            bg_processes[num_bg_processes] = pid;
            num_bg_processes++;
        } else {
            printf("Maximum number of background processes reached.\n");
        }
        
    }
}

// Function to bring a background process to the foreground
void bring_to_foreground(char** args,int* position) {
	
	// handle no input to fg
	if (number_of_token == 1){
		printf("fg: Expect argument\n");
		return;
	}
	
    // Extract the PID from the command
    pid_t pid = atoi(args[(*position)+1]); 
    
    // handle string input instead of pid
	if (pid == 0){
		printf("fg: Expect pid\n");
		return;
	}
	
	// handle garbage number
	if (num_bg_processes == 0){
		printf("fg: %d: no such job\n", pid);
	}
	
    // search for pid to bring back the process
    for (int i = 0; i < num_bg_processes; i++) {
        if (bg_processes[i] == pid) {
		
			// bring the background process to the foreground			
			printf("Bringing background process with PID %d to foreground\n", pid);
			waitpid(pid, NULL, 0); // Wait for the process to complete

			// Remove the process from the list of background processes
            for (int j = i; j < num_bg_processes - 1; j++) {
                bg_processes[j] = bg_processes[j + 1];
            }
            num_bg_processes--;
            break;
        }
        // if the pid not found 
        else if (i == num_bg_processes-1){
        	printf("fg: %d: no such job\n", pid);
        }
    }
    
}

// Execute the command normally
void execute_command(char** args){

    // Execute the command
    int status= 0;
    
    pid_t pid = fork(); // Create a new process
    
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("My-shell"); // Print error if execvp fails
        }
        exit(EXIT_FAILURE); // Exit child process
        
    } else if (pid < 0) {
        // Forking error
        perror("fork");
        
    } else {
		// Parent process
        foreground_pid = pid; // Set the foreground process PID
        
        // Parent process waits for child to complete
        do {
            waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
        
        foreground_pid = -1; // Reset foreground process PID
    }
    
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

// Signal handler for both SIGINT (Ctrl+C) and SIGTSTP (Ctrl+Z)
void signal_handler(int signal) {
    if (signal == SIGINT) {
        printf("\nTerminated using (Ctrl+C).\n");
        
    } else if (signal == SIGTSTP) {
        printf("\nStoped using (Ctrl+Z).\n");
        
		// Check if there's a foreground process running
		if (foreground_pid > 0) {
		    printf("\nSIGTSTP received. Process %d moved to background.\n", foreground_pid);
		    
		    // Add background process to list
		    if (num_bg_processes < MAX_BG_PROCESSES) {
		        bg_processes[num_bg_processes] = foreground_pid;
		        num_bg_processes++;
		    } else {
		        printf("Maximum number of background processes reached.\n");
		    }
		    
		    foreground_pid = -1; // Reset foreground process PID
		    
		} else {
		    printf("\nNo foreground process to move to background.\n");
		}
		
		// Make sure the message is printed immediately
		fflush(stdout); 
		    
    }
}
