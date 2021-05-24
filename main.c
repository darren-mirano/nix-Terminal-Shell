/*
    UMBC CMSC 421
    Spring 2021
    Project 1

    Due Date: 2/28/21 11:59:00 pm

    Author Name: Darren Mirano
    Author email: dar6@umbc.edu
    Description: a simple linux shell designed to perform basic linux commands
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <ctype.h>
#include "utils.h"

/*
    In this project, you are going to implement a number of functions to
    create a simple linux shell interface to perform basic linux commands.
    Please note that the below set of functions may be modified as you see fit.
    We are just giving the following as a suggestion. If you do use our
    suggested design, you *will* need to modify the function signatures (adding
    arguments and return types).
*/

/*
DEFINE FUNCTION PROTOTYPES (add arguments/return types as needed)
user_prompt_loop()
get_user_command()
parse_command()
execute_command()
*/

int user_prompt_loop();
char *get_user_command();
char **parse_command(char *);
int execute_command(char*, char **);
int free_array(char **);

const char *proc_str = "proc";
const char *exit_str = "exit";

int main(int argc, char *argv[])
{
	/*
        Write the main function that checks the number of arguments passed to
        ensure no command-line arguments are passed; that is to say that if the
        number of arguments is greater than 1 then exit the shell with a message
        to stderr and return value of 1. Otherwise call the user_prompt_loop()
        function to get user input repeatedly until the user enters the "exit"
        command.
    */

	user_prompt_loop();
	return 0;
}

/*
    user_prompt_loop():
    Get the user input using a loop until the user exits, prompting the user for
    a command. Gets command and sends it to a parser, then compares the first
    element to the two built-in commands ("proc", and "exit"). If it's none of
    the built-in commands, send it to the execute_command() function. If the
    user decides to exit, then exit 0 or exit with the user given value.
*/
int user_prompt_loop()
{
	/*
      loop:
        1. prompt the user to type command by printing "$ " (don't forget the
           space)
        2. get the user input using get_user_command() function
        3. parse the user input using parse_command() function
        Example:
            user input: "ls -la"
            parsed output: ["ls", "-la", NULL]
        4. compare the first element of the parsed output to "proc"and "exit"
        5. if the first element is "proc" then you have to use the /proc file
           system as described in part 2 of the project
            i) concat the full command:
                Ex: user input $ proc /process_id_no/status
                    concated output: /proc/process_id_no/status
            ii) read from the file line by line. you may user fopen() and
                getline() functions
            iii) display the following information according to the user input
                 from /proc
                a) Get the cpu information if the input is /proc/cpuinfo
                - Cpu Mhz
                - Cache size
                - Cpu cores
                - Address sizes
                b) Get the number of currently running processes from
                   /proc/loadavg
                c) Get how many seconds your box has been up, and how many
                   seconds it has been idle from /proc/uptime
                d) Get the following information from /proc/process_id_no/status
                - the vm size of the virtual memory allocated the vbox
                - the most memory used vmpeak
                - the process state
                - the parent pid
                - the number of threads
                - number of voluntary context switches
                - number of involuntary context switches
                e) display the list of environment variables from
                   /proc/process_id_no/environ
                f) display the performance information if the user input is
                   /proc/process_id_no/sched
        6. if the first element is "exit" the use the exit() function to
           terminate the program
        7. otherwise pass the parsed command to execute_command() function
        8. free the allocated memory using the free() function
    */

	/*
        Functions you may find useful here (not an exhaustive list):
        get_user_command(), parse_command(), execute_command(), strcmp(),
        strcat(), strlen(), strncmp(), fopen(), fclose(), getline(), isdigit(),
        atoi(), fgetc()
    */

	while (1) {
		char *command;
		char *first_index;
		char **parsed;

		printf("$ ");

		command = get_user_command();

		parsed = parse_command(command);

		first_index = parsed[0];

		if (first_index == NULL){
			//skip since unescape failed
			free(command);
			free(parsed);

		} else {
			int proc_cmp = strcmp(proc_str, first_index);
			int exit_cmp = strcmp(exit_str, first_index);

			if (proc_cmp == 0) {
				//Count number of arguments since proc only
				//takes two arguments
				char* var = parsed[0];
				int count = 0;

				while (var != NULL){
					count++;
					var = parsed[count];
				}

				if (count > 2){
					printf("Invalid number of arguments\n");
					
				} else {

					char *location = parsed[1];

					//Format correctly and concatinate
					char proc[100] = "/proc/\0";
					char *proc_cmd = strcat(proc, location);

					FILE *file;
					char *curr_line = NULL;
					size_t temp_size;

					file = fopen(proc_cmd, "r");
					if (file == NULL) {
						printf("File not found\n");
						
					} else {
						while (getline(&curr_line, &temp_size, file) !=
							-1) {
							printf("%s", curr_line);
						}
						fclose(file);
					}

					free(curr_line);
				}

			} else if (exit_cmp == 0) {
				int num_elements = 0;
				char *pos = parsed[num_elements];

				while (pos != NULL) {
					num_elements++;
					pos = parsed[num_elements];
				}

				if (num_elements == 1) {
					free(command);
					free_array(parsed);
					free(parsed);

					exit(0);

				} else if (num_elements == 2) {
					int len = strlen(parsed[1]);
					int check_num = 0;

					//Check if all characters in string are numbers. This handles
					//the case if there is an argument with letters and numbers.
					for (int i = 0; i < len; i++) {
						if (isdigit(parsed[1][i]) == 0) {
							check_num = 1;
						}
					}

					if (check_num ==
						0) { //Argument is a valid number
						char *convert = parsed[1];
						int val = atoi(convert);

						free(command);
						free_array(parsed);
						free(parsed);
						exit(val);
					} else { //Argument is not a valid number
						//Skip and rempromt user for a command
					}
				} else {
					//exit only accept 0 or 1 arguments, skip
				}

			} else {
				execute_command(command, parsed);
				
			}
			//Free after each command has completed
			free(command);
			free_array(parsed);
			free(parsed);
		}
	}
	return 0;
}

/*
    get_user_command():
    Take input of arbitrary size from the user and return to the
    user_prompt_loop()
*/
char *get_user_command()
{
	/*
        Functions you may find useful (not an exhaustive list):
        malloc(), realloc(), getline(), fgetc()
    */

	size_t temp_size = 1;
	char *string = NULL;
	string = (char *)malloc(temp_size);
	getline(&string, &temp_size, stdin);

	//Replace newline character with (null)
	int len = strlen(string);
	if (string[len - 1] == '\n') {
		string[len - 1] = 0;
	}
	return string;
}

/*
    parse_command():
    Take command input read from the user and parse appropriately.
    Example:
        user input: "ls -la"
        parsed output: ["ls", "-la", NULL]
    Example:
        user input: "echo     hello                     world  "
        parsed output: ["echo", "hello", "world", NULL]
*/
char **parse_command(char *command)
{
	/*
        Functions you may find useful (not an exhaustive list):
        malloc(), realloc(), free(), strlen(), first_unquoted_space(),
        unescape()
    */

	char *tmp_cmd = command;
	int num_args = 0;
	int index = 0;

	//Count how many arguments (or words) the user entered
	//This is used when allocating memory for the char ** array
	while (index != -1) {
		index = first_unquoted_space(tmp_cmd);
		
		//Check if first word has spaces before
		if (index == 0){
			while (isspace(tmp_cmd[0])){
				tmp_cmd++;
			}
		} else {
			if (index == -1) {
				num_args++;
			} else {
				tmp_cmd = tmp_cmd + index;
				num_args++;

				while (isspace(tmp_cmd[0])) {
					tmp_cmd++;
				}
				if (tmp_cmd[0] ==
					'\0') { //Accounts for spaces after last word
					num_args = num_args - 1;
				}
			}
		}
	}

	char **arr = calloc(num_args + 1, sizeof(char *));
	char *tmp_cmd2 = command;

	//Loop over each command and append them to the array, making sure
	//to free any unwanted memory leaks.
	for (int i = 0; i < num_args; i++) {
		int first_space = first_unquoted_space(tmp_cmd2);

		//Check if first word has spaces before
		if (first_space == 0){
			while(isspace(tmp_cmd2[0])){
				tmp_cmd2++;
			}

			//Subtract 1 from i since you dont wan't it to incriment
			i = i - 1;
		} else {

			if (first_space == -1) {
				int len = strlen(tmp_cmd2);
				//len + 1 makes space for the null terminator
				char *substr = calloc(len + 1, sizeof(char));
				strncpy(substr, tmp_cmd2, len);
				substr[len] =
					'\0'; //strncpy doesn't add null terminator
				char *unescaped_substr = unescape(substr, stderr);

				arr[i] = unescaped_substr;

				free(substr);

			} else {
				char *substr = calloc(first_space + 1, sizeof(char));
				strncpy(substr, tmp_cmd2, first_space);
				substr[first_space] = '\0';
				char *unescaped_substr = unescape(substr, stderr);
				arr[i] = unescaped_substr;

				free(substr);

				//Get to next argument. If there are spaces, skip them
				tmp_cmd2 = tmp_cmd2 + first_space;
				while (isspace(tmp_cmd2[0])) {
					tmp_cmd2++;
				}
			}
		}
	}

	arr[num_args] = '\0'; //Add null terminator to end or array

	return arr;
}

/*
    execute_command():
    Execute the parsed command if the commands are neither proc nor exit;
    fork a process and execute the parsed command inside the child process
*/
int execute_command(char* input, char **command)
{
	/*
        Functions you may find useful (not an exhaustive list):
        fork(), execvp(), waitpid(), wait()
    */

	pid_t child_pid = fork();
	int status;

	char *cmd = command[0];

	if (child_pid == -1) { //Fork failed
		exit(1);
	} else if (child_pid == 0) {
		int check = execvp(cmd, command);

		//If execvp fails, kill the child.
		if (check == -1) {
			free(input);
			free_array(command);
			free(command);
			exit(1);
		}
	} else {
		while (wait(&status) != child_pid) {
			//waiting
		}
	}
	return 0;
}

//Function to free an array of char*'s
int free_array(char **arr)
{
	int i = 0;
	int count = 0;
	char *string = arr[i];

	while (string != NULL) {
		i++;
		count++;
		string = arr[i];
	}

	for (int i = 0; i < count; i++) {
		free(arr[i]);
	}
	return 0;
}
