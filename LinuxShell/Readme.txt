Implementation and Design Description
                  Ruiyuan Jiang
1.	As this shell must prepend the name of the CWD to the command before executing it. A sub-function type_promt is written.  Getpwuid ,geuid, and gethostname are used to finish this sub-function. And put it in the while function to print it before every command.

2.	As the shell need to read the command on the terminal, a read line function is written. I use the getline function in the library to get the stdin on the terminal.

3.	In this step, the read in command must be examined and judge whether this command is legal or not. So I write a analyzing sub-function. As we only got three different command type, this function return different value to represent different property of the command.

4.	According to the return value from the syntax analyzer, I switch the command to different sub-function such as redirection, pipe_command, simple command, illegal input.

5.	In the simple command function, I also implement the simple exit function by return a zero code to the main function. I use this zero code to break the while loop in the main function.
 
6.	Pipe_command , redirection, and simple command function, all these functions, have the same structure. Firstly, they read in the command, and separate the command into tokens according to
 different rule. Pipe_command and redirection will do some extra job in changing the output and input port before using system call to execute the command.

7.	The while loop in the main function which keeps the program running is based on the return value of the above three sub-function.

