# 380Project0

1pt Extra Credit: 
What does the man page have to say about mixing calls to sleep(3) and alarm(2)? Answer this
question in your README: it should include a sample program.

The man page says that mxining calls to sleep(3) and alarm(2) are a bad idea. The scheduling delays can cause the execution of the process to be delayed by an arbitrary amount of time. 

For example imagine setting an alarm for 5 seconds, but then sleeping for 10 seconds. The alarm will go off after 5 seconds even though the program was suppose to sleep for 10. 

int main(){
	signal(SIGALRM, handler)
	alarm(5);
	sleep(10);
}


What other signal(s) have this property? Which signals will terminate the program if unhandled? Which man page has this information? 

SIGSTOP and SIGKILL cannot be ignored as explained by the signal(2) man page.
