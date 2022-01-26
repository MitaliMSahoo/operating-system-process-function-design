# operating-system-process-function-design
The program does the function of forking child and grandchild processes and does three functions as below: 
	a.	Pipe operation : It does the work of data writing by a process and then shared to its child to do read operation.
	b.	Redirection of input and output: Here the child process reads the input from the user and redirects it into a file(as an output)
	c.	Background job run: The child of a process runs a job function in the background.


2. High-level design(you could use a flowchart for this)
    Skeleton of the processes running:
				         P A R E N T (MAIN)
			          	/	           | 	                   \
		      (C1) pipe_fork                (C2)bg_fork           (C3)redirect_fork
				/     	             \
                  		(GC1)             		(GC2)
	1.	The program creates 3 child process of a parent process called  pipe_fork(C1) , bg_fork(C2) & redirect_fork (C3).
	2.	Child process pipe_fork(C1)  does the function of performing pipe operation.
		(i) 	It creates a child (GrandChild(GC1) for parent process)  to which the process(C1) sends data(eg: HELLO WORLD) in written.
		(ii) 	The GC1 receives the data from C1 and reads it and displays on screen.
	3.	Child process bg_fork(C2)  does the function of running a job in the background.
		(i) 	C2 forks a child process of its own (GrandChild(GC2)) and GC2 runs a job in the background.
	4.	Child process redirect_fork (C3) performs the operation of redirection of input and output.
		(i) 	C3 takes input from the user and redirects the data into a file called “redirect.txt”.

3. Building the program
	1.	Variables used: The variables are mostly used locally inside the functions and are locally passes between every parent and its child.
	2.	Functions Used:
		a.	void redirect_fork_function(void) : Executes input and output redirection code.
		b.	void bg_fork_function(void) : Created a grandchild (GC2) for child redirect_fork (C2) and performs background job function execution
		c.	void bg_process(void) : Shows that a background task as running.
		d.	void pipe_fork_function(void) : Created a grandchild (GC1) for child pipe_fork (C1) and performs pipe operation.
		e.	int main(int argc, char * argv[]) : Creates 3 child process (C1) pipe_fork, (C2)bg_fork, (C3) redirect_fork
	3.	Logic:
		f.	When the main function starts in the program, the parent function(MAIN) is the one that’s running.
		g.	Fork() is called and a child process pipe_fork is created. Conditions are provided to check which process is currently running (parent or the child C1). 
		h.	If child C1 is running( pipe_fork == 0) , then a new function pipe_fork_function() is called.
			i.	pipe_fork_function() function creates a child process with fork(), named grandchild (GC1).
			ii.	Condition is provided to check if C1 or GC1 is currently running. If child (C1 ) is active, it writes some data (“HELLO WORLD”) into variable 					fd[1].
			iii.	If grandchild(GC1) is active, it tries to read the data from fd[0]. fd variable is generated by pipe() call.
		i.	If only Parent process is running (condtion: pipe_fork > 0),  redirect_fork process (C3) is forked from parent. A function redirect_fork_function() is 				called.
			i.	redirect_fork_function() function prompts input from an user and redirects it as an output to a file.
			j.	If only parent process is running and pipe_fork and  redirect_fork processes are inactive (condition: pipe_fork >0 && redirect_fork > 0), bg_fork 				 (C2) process is forked() from the parent process. 
			i.	bg_fork() function created a child process with fork(), named grandchild (GC2).
			ii.	Condition is provided to check if C2 or GC2 is currently running. When GC2 process is running, a function bg_process() is called which runs the 				background job function.


