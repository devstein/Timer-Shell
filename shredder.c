#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define BUFFER_SIZE 1024
int pid;

void handler(int signum) {

	if (signum == SIGALRM) {
		const char timeout_msg[] = "Bwahaha ... tonight I dine on turtle soup\n";
		if (write(1, timeout_msg, sizeof(timeout_msg)) == -1){
			perror("Failed to write.");
		}	
		if (kill(pid, SIGKILL) == -1){
			perror("Failed to kill.");
		}
	}
	//CTRL + C
	else if (signum == SIGINT){
		if (kill(pid, SIGKILL) == -1){
			perror("Failed to kill.");
		}
	}
	else {
		exit(3);
	}
}

void setTimer(int time){
	if (signal(SIGALRM, handler) == SIG_ERR){
		perror("Signal error");
	}
	alarm(time);
}

void parseCommand(char *input, char *cmd){
	//parse cmd
	//remove spaces and tabs
	while(*input == ' ' || *input == '\t'){ input++; }

	while(*input != ' ' && *input != '\0' && *input != '\n'){
		*cmd++ = *input++;
	}
	//set end of cmd
	*cmd = '\0';
}

int main(int argc, char * argv[]) {

	const char prompt[] = "shredder# ";
	int time; 

	int stuff;
	//read input
	if (argv[1] != NULL){ 
		time = atoi(argv[1]); 

		//do run for invalid input
		if(time <= 0){
			const char bad_timer[] = "Invalid input. Set a numerical timer of at least one second\n";
			if (write(1, bad_timer, sizeof(bad_timer)) == -1) {
				perror("Unable to write");
			}
			exit(3);			
		}
	}
	else{ time = 0; }

	while(1){

		//prompt and wait for cmd
		write(1, prompt, sizeof(prompt) - 1);
		
		//set signal for CTRL + C
		signal(SIGINT, handler);

		char buffer[BUFFER_SIZE];
		int num_read = read(0, buffer, BUFFER_SIZE);

		//handle EOF
		if (num_read == 0){
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
			if (wait(NULL) == -1){
				perror("Wait failed.");
			}
			//clear timers
			alarm(0);
			pid = 0;
		}

	}
}