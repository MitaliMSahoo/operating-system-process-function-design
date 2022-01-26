#include <sys/types.h>		 // Header file containing fork() operation 

#include <sys/wait.h>			//header file for wait

#include <unistd.h>

#include <stdio.h>

#include <stdlib.h>

#include  <string.h>

#include <fcntl.h>

#define BUFFERSIZE 100     // defining buffer for storing pipe variable


/*  prog skeleton:

                P A R E N T (MAIN)
               /       |           \
     (C1) pipe_fork    (C2)bg_fork   (C3)redirect_fork
         /              \
      (GC1)             (GC2)
       

*/

void redirect_fork_function(void) {			//function def to perform i/p, o/p redirection operation 
  int statuschk;
  FILE * redirect_fd;
  char buffer1[BUFFERSIZE];
  fgets(buffer1, BUFFERSIZE, stdin);	//read input from user
  redirect_fd = fopen("redirect.txt", "w");	//opening a new file for writing
  if (redirect_fd == NULL) {	//check if file is created 
	printf("C2: Error opening file OR couldnt create new file");
	exit(1);
  }
  fprintf(redirect_fd, "%s", buffer1);	//write to the file newly created 
  printf("C2: Done redirecting input to a file\n");
  waitpid(-1, & statuschk, 0);
}

void bg_process(void) {			//func def to check if background proc running
  printf(" GC2: Grandchild: i'm running in the background\n");
}

void bg_fork_function(void) {  		//func def to perform BG run operation 
  // call child that executes bg_process()
  int grandchild = fork();   // child created 
	if (grandchild == 0) // checking if grandchild process is created
	{
    	bg_process();			//call a func to run bg process 
	} else if (grandchild < 1) {
  	perror("C3: Creation of grandchild Failed!!\n");
	} else {
     	printf("C3: PID: %d : BG_JOB_FORK waiting for grandchild to finish output redirection function \n",getpid());
 	 
	}
 
}



void pipe_fork_function(void) {
  int n, fd[2];
  // int statuschk;
  char buffer[100];
  if (pipe(fd) < 0) {							//check if pipe() call is success
	perror("C1: Pipe could not created\n");
  } else {
	// Creating grandchild process  to handle pipe operation
	int grandchild = fork();		
	if (grandchild == 0) // checking if child process is created
	{
  	printf("GC1: pipe (grandchild) running and receiving data from parent\n");
  	n = read(fd[0], buffer, 100);    //grandchild reading the data sent by its parent proc 
  	write(1, buffer, n);
  	printf("\n");

	} else if (grandchild < 1) {
  	perror("C1: creation of pipe *(grandchild) Failed!!\n");
	} else {
        
  	//printf("C1: pipe_fork_function is currently active with PID: %d\n", getpid());
  	printf("C1: Passing data to grandchild \n");
	// Child pro writing data onto pipe 
  	write(fd[1], "C1: HELLO WORLD (message to grandchild) ", 40);	
  	// waitpid(-1, & statuschk, 0);
      	while(wait(NULL) > 0);

	}

  }
}

int main(int argc, char * argv[]) {
  pid_t pipe_fork, redirect_fork, bg_fork;		//variables for storing fork status 
  FILE * redirect_fd;					// file initialization for output redirection 
  printf("MAIN: Forking Process started ....\n");
  pipe_fork = fork(); 					// create child process for pipe operation
  if (pipe_fork > 0) {					// check if pipe_fork process is inactive
	redirect_fork = fork();				// create child process for redirection operation
  }
  if (pipe_fork >0 && redirect_fork > 0) {	//check if pipe_fork & redirect_fork process are inactive and only parent is running, then fork new child as below
	bg_fork = fork();				// create child process for BG operation
  }
  if (pipe_fork < 0)		// check if pipe_fork child got created 		
	printf("error");
  else if (pipe_fork == 0)             // check if pipe_fork is active
  {
	printf("MAIN: pipe_fork_function child created with PID: %d \n", getpid());
	pipe_fork_function();
  }
  else if (redirect_fork == 0 && pipe_fork > 0) {    // check if redirect_fork child process is active
	printf("MAIN: redirect_fork child created with PID: %d \n", getpid());
	redirect_fork_function();      
  }
  else if (bg_fork == 0 && pipe_fork > 0 && redirect_fork > 0)  // check if bg_fork is active
  {
	printf("MAIN: bg_fork child created with PID %d \n",getpid());
	bg_fork_function();
        //printf("MAIN: bg_fork child created \n");
  } 
  else {    // check if parent process is active
	printf("MAIN: PID %d Parent Process running and waiting for child process to finish.\n", getpid());
	while(wait(NULL) > 0);
	printf("MAIN: PID %d Parent process is exiting. \n", getpid());
	exit(0);			// exit the parent process
  }
  
  return 0;
}




