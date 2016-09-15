#include <signal.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/wait.h>

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

//can only use the following calls

//execve (2)
//fork (2)
//wait (2)
//read (2)
//write (2)
//signal (2)
//alarm (2)
//kill (2)
//exit (2)

//malloc(3)
//free(3)
//atoi once
//perror(3)

//use clang to compile
//timeout = 0 is an invalid input

#define BUFFER_SIZE 1024

void handler(int signum) { 

	//check to see if child process is running. if so kill it
	const char timeout_msg[] = "Bwahaha ... tonight I dine on turtle soup\n";
	const char alarm_msg[] = "HANDLING ALARM...\n";
	write(2, alarm_msg, sizeof(alarm_msg) - 1);

	_exit(2); 

}



//argc is nubmer of arguments INCLUDING name of program (so always 1)
//argv[0] is shredder

int main(int argc, char * argv[]) {

	const char msg[] = "shredder# ";
	const char new_line[] = "\n";

	//no argv means no timer
	int time;
	if (argv[1] != NULL){
		time = atoi(argv[1]);
	}
	else{
		time = 0;
	}

	//signal returns previous value of signal handler
	signal(SIGALRM, handler);

	//arranges for SIGALRM to be delivred to the calling process in (seconds)
	// if (time > 1) {
	// 	alarm(time);
	// }

	// char *test[] = {"/bin/cat", "hello"};

	while(1) {

		//wait for and read command
		write(2, msg, sizeof(msg) - 1);
		char buffer[BUFFER_SIZE];
		int num_read = read(0, buffer, BUFFER_SIZE);

		//parse cmd up to space or new line
		int last_char = 0;
		int first_char = 0;
		//remove spaces and tabs before
		while(buffer[last_char] == ' ' || buffer[last_char] == '\t'){
			last_char = last_char + 1;
			first_char = first_char + 1;
		}
		while(buffer[last_char] != ' ' && last_char < num_read){
			last_char = last_char + 1;
		}
		int cmd_length = last_char - first_char + 1;
		char cmd[sizeof(char) * (cmd_length)];

		for (int i = 0; i < cmd_length; i++){
			cmd[i] = buffer[first_char + i];
		}

		int pid = fork();


		if (!pid){
			write(2, cmd, sizeof(cmd) - 1);

			char *test[] = {cmd};

			execve(cmd, test, NULL);
			//set timer
			if (time > 0) {
				alarm(time);
			}
		} else{
			wait(NULL);
		}
	}
}


// 1pt Extra Credit: What does the man page have to say about mixing calls to sleep(3) and alarm(2)? Answer this
// question in your README: it should include a sample program.

// 1pt Extra Credit: What other signal(s) have this property? Which signals will terminate the program if unhandled? Which
// man page has this information? Answer these questions in your README

// 5pt Extra Credit: For extra credit, write a simple parser that will handle arbitrary arguments and pass them to execve
// appropriately. Note, you cannot use any function from the C standard library. This includes everything from string.h, so don’t

// 3pt Extra Credit: For extra credit, implement your own version of atoi(3) that can handle arbitrary numbers within the 32
// bit width of an integer.