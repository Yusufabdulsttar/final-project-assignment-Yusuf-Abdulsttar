/* 
 * File:   my-shell.c
 * Author: Yusuf Abdulsttar
 *
 * Created on jan 30, 2024, 1:48 PM
 */
#include "my-shell.h"

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("myshell> ");  // Print the prompt

		//read the command
		if(read_command(command,MAX_COMMAND_LENGTH)){
			break;
		}
        
        // Process the command
        // (For now, just print it)
        printf("You entered: %s", command);
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
