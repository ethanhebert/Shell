Karina Chang, Ethan Hebert

techshell.c is a shell that supports I/O redirection with built-in support for the commands cd, pwd, and exit.

The shell contains a while loop that will continually check for input until the user types "exit", which will then 
exit the script

Inside the loop, the program calls the current working directory and displays it to the user,
mimiking a real terminal. 

Next, the character arrays tokens and inout are initialized.
The array tokens refers to the user input, while inout refers to which parts
would be used for input and output.

The function gettokens is called which tokenizes the user input and places each 
string in their respective arrays.

initialTest is called. This function returns an integer, which is used to indicate when to
exit the while loop and end the program. 

initialTest compares the initial user input for cd, pwd, or exit, and executes the commands
accordingly. If none of these strings matches the first user comand, another function, forkTest,
is called.

forkTest creates a forked child process that will execute the given command. Before using execvp,
the array inout is used to determine what files will be input or output.If execvp fails an error
message will be displayed in the parent.

