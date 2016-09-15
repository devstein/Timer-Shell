#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_SIZE 1024
int pid;


const char test_msg[] = "testing";


void handler(int signum) {

	if (signum == SIGALRM) {
		const char timeout_msg[] = "Bwahaha ... tonight I dine on turtle soup\n";
		write(1, timeout_msg, sizeof(timeout_msg));	
		kill(pid, SIGKILL);
	}
}

void setTimer(int time){
	signal(SIGALRM, handler);
	alarm(time);
}

void parseCommand(char *input, char *cmd){
	//parse cmd
	//remove spaces and tabs
	while(*input == ' ' || *input == '\t'){ input++; }

	while(*input != ' ' && *input != '\0' && *input != '\n'){
		printf("ASSIGNING %c\n", *input);
		*cmd++ = *input++;
	}
	//set end of cmd
	*cmd = '\0';
}

void resetBuffer(char *buff){
	while(*buff != '\0'){
		*buff++ = '\0';
	}
}

int main(int argc, char * argv[]) {

	const char prompt[] = "shredder# ";
	int time; 

	//read input
	if (argv[1] != NULL){ time = atoi(argv[1]); }
	else{ time = 0; }

	//make sure argv[1] is int

	//do not run for negative timers
	if (time < 0) {
		const char bad_timer[] = "Invalid input: Cannot set a negative timer\n";
		write(1, bad_timer, sizeof(bad_timer));
		exit(3);
	}

	while(1){
		//prompt and wait for cmd
		write(1, prompt, sizeof(prompt) - 1);
		
		char buffer[BUFFER_SIZE];
		int num_read = read(0, buffer, BUFFER_SIZE);

		//handle EOF
		if (num_read == 0){
			write(1, "Exit\n", sizeof("Exit\n"));
			exit(3);
		}
		else if (num_read == -1){
			perror("Unable to read input.");
		}

		char cmd[num_read];

		parseCommand(buffer, cmd);

		pid = fork();

		if (pid == 0){

			char * ex_args[] = {cmd, NULL};

			int err = execve(cmd, ex_args, NULL);

			if (err < 0 ){
				perror("Error");
			}
			exit(3);

		} else if (pid < 0){
			perror("Fork Error");
			exit(3);
		}
		else{ 
			//set alarm
			setTimer(time);
			wait(NULL);
			//clear timers
			alarm(0);
		}

	}
}