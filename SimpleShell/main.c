/* 
 * ---------------------------------------------------------------------------
 * Simple Shell main.c
 * 2019 (c) Thomas Young
 * --------------------------------------------------------------------------- 
 */

#include <stdio.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <unistd.h>
#include <string.h>

void simpleshell(void);

int shell_process_line(char **args);
int shell_run(char **args, char *path);
int execute(char **args);

char **readline(void);
char **change_args(char **args);

void shell_help(void);
void shell_goto(char **args);
void shell_wai(void);
void shell_exit(void);

/* Calls the main shell Function */

 int main(void){
	
	simpleshell();

	return 0;
}	 

/* Loops the input and process function indefinetly*/

void simpleshell(void){
	
	printf("Simple Shell. Build: DEV. Thomas Young (c) 2019\n");
	printf("A Young Enterprise Application.\n");

	char *command;
	char **args;

	while(1){
		
		printf(">");
		args = readline();
		
		if(args[0] == NULL){
			continue;
		} 
		else {
			shell_process_line(args);
		}	
	}
	return;
}

/* Checks to see if the user specified a shell command, if so it is executed with the arguments being passed
 * to the function, if not, it is passed to a fucntion that will look for and then execute programs */

int shell_process_line(char **args){

		if(strcmp(args[0], "help") == 0){
			shell_help();
		} 
		else if(strcmp(args[0], "goto") == 0){
			shell_goto(args);
		} 
		else if(strcmp(args[0], "wai") == 0){
			shell_wai();
		} 
		else if(strcmp(args[0], "run") == 0){
			char path[64] = "./";
			strcat(path, args[1]);
			shell_run(args, path);
		} 
		else if(strcmp(args[0], "exit") == 0){
			shell_exit();
		}
		else {
			if(execute(args) == 0){
			}
		}
	return 0;
}

/* Moves the arguments forward so it can be passed to function run to execute with the correct 
 * program arguments */

 char **change_args(char **args){
	 int i = 0;
	 for(i = 0; i < 32; i++){
		 args[i] = args[i + 1];
		 if(args[i] == NULL){
			 break;
		 }
	 }
	 return args;
 }

/* Executes the program appending the path infront so it can be executed by excvp() */

int shell_run(char **args, char path[64]){

	args = change_args(args);

	pid_t pid = fork();
	if(pid < 0){
		printf("ssl: Failed to fork to execute program. I'm afraid it's all over...\n");
		return 0;
	} else if(pid == 0){		//Is child process
		if(execv(path, args) == -1){
			printf("ssl: Couldn't execute program\n");
		}
	}

	wait(NULL);

	return 0;
}

int execute(char **args){

	pid_t pid = fork();
	if(pid < 0){
		printf("ssl: Failed to fork to execute program. I'm afraid it's all over...\n");
		return 0;
	} else if(pid == 0){		//Is child process
		if(execvp(args[0], args) == -1){
			printf("ssl: Couldn't execute program\n");
		}
	}

	wait(NULL);

	return 0;
}

/* Displays a help message listing the built-in shell commands */

void shell_help(void){
	printf("--SimpleShell Help--\n");
	printf("Command:\tWhat it does:\n");
	printf("goto\t\tChanges the shell's directory or folder	to that specified, takes one argument which is file path.\n");
	printf("wai\t\tAKA Where am I. Displays the current location of the shell in the filesystem, takes no arguments.\n");
	printf("run\t\tExecutes a given program, takes one argument which is the path to this.\n");
	printf("exit\t\tExit's the shell\n");
	printf("help\t\tDisplays this text on the basic built-in commands of SimpleShell.\n");
	return;
}

void shell_goto(char **args){
	if(args[1] == NULL){
		printf("No path specified\n");
		return;
	}
	else if(chdir(args[1]) != 0){
		printf("Failed to navigate to path: %s\n", args[1]);
	}
	return;
}

void shell_wai(void){
	char location[255];
	getcwd(location, sizeof(location));
	printf("Current Location: %s\n", location);
	return;
}

void shell_exit(void){
	exit(0);
}

char **readline(void){

	char **args = malloc(64 * sizeof(char *));
	char *command;	

	char c;
	char buffer[1024];

	int count = 0;

	while(1){
		
		c = getchar();
		
		if(c == EOF || c == '\n'){
			buffer[count] = '\0';
			command = buffer; 
			break;

		} else if(count > 1023){
			printf("Oh no! The shell can only take commands up to 1024 characters long!\n");
			return NULL;
		
		} else {
			buffer[count] = c;
		}
		count++;
	}
	
	count = 0;

	char *argument = strtok(command, " ");

	while(argument != NULL){
		args[count] = argument;
		argument = strtok(NULL, " "); 
		count++;
	}
	args[count] = NULL;
	return args;
}	
