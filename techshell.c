/*
Ethan Hebert and Karina Chang
CSC-222-003
TechShell
2-20-22
*/

#include <stdio.h> //for input/output
#include <stdlib.h> //standard library of functions
#include <unistd.h> //for pid/fork
#include <sys/wait.h> //for wait
#include <string.h> //for fflush
#include <errno.h> //for errno

#define INPUT_MAX 512
#define NUMARGS 20

void printcwd(char* end);
void gettokens(char** arr, char** inout);
void forkTest(char** arr, char** inout);
int initialTest(char** arr, char** inout);

int main(int argc, char* argv[])
{
	int loop = 1;
	while(loop)
	{
		//print cwd
		printcwd("$ ");
		
		//get tokenized input
		//check for < or >
		//inout[0] = input "<" location, inout[1] = output ">" location in array of tokens
		char** tokens = (char**) malloc(NUMARGS*sizeof(char*));
		char** inout = (char**) malloc(2*sizeof(char*));
		inout[0] = "";
		inout[1] = "";
		
		gettokens(tokens, inout);
		
		//test for cd, pwd, exit, else call forked process
		loop = initialTest(tokens, inout);
	}
	exit(0);
}



//FUNCTIONS START HERE

void printcwd(char* end)
{
	char cwd[256];
	if (getcwd(cwd, sizeof(cwd)) == NULL)
	{
		printf("Error with current working directory.\n");
		exit(1);
	}
	else
	{
		printf("%s%s", cwd, end);
	}
}


void gettokens(char** arr, char** inout)
{
	//read the user input and store in the string
	char input[INPUT_MAX];
	char* str1 = fgets(input, INPUT_MAX, stdin);
	input[strlen(input) - 1] = '\0'; //delete the newline character at the end of fgets
	
	//tokenize the string at every space
	int curr = 0; //current argument
	char* piece = strtok(input, " "); //this returns only the first token stored under piece
	arr[curr] = piece; 
	piece = strtok(NULL, " "); //get the next token of the string
	
	while (piece != NULL) //loop through all tokens in the string
	{

		if (strcmp(piece, "<") == 0)
		{
			piece = strtok(NULL, " "); //get the next token of the string
			inout[0] = piece;
		}
		else if (strcmp(piece, ">") == 0)
		{
			piece = strtok(NULL, " "); //get the next token of the string
			inout[1] = piece;
		}
		else
		{
			curr++;
			arr[curr] = piece;
		}
		piece = strtok(NULL, " "); //get the next token of the string
	}
	//done so we can later use execvp in forked process
	curr++;
	arr[curr] = NULL;
}

int initialTest(char** arr, char** inout)
{
	int loop = 1;
	char* str1 = arr[0];
	if (strcmp(str1, "pwd") == 0) //pwd
		printcwd("\n");
	
	else if (strcmp(str1, "exit") == 0) //exit
		loop = 0;
		
	else if (strcmp(str1, "cd") == 0) //cd
	{		
		char* cdarr = strdup(arr[1]); //move arr[1] into a character array to check for ~
		
		if (cdarr[0] == '~') //home directory
		{
			if (cdarr[1] == '\0')
				chdir(getenv("HOME"));
			
			else if (cdarr[1] == '/')
			{
				//the final array for chdir to use - home env var + /Desktop/etc.
				char* finaldest = strdup(getenv("HOME")); 
				
				char* cdarr2 = cdarr + 1; //cdarr2 holds the new directory without the ~
				strcat(finaldest, cdarr2);
				
				if (chdir(finaldest) != 0) //successful change
					printf("ERROR %d (%s)\n", errno, strerror(errno)); //error in changing
			}
			
			else  
				printf("ERROR %d (%s)\n", 2, strerror(2)); //error in changing
		}
		
		else if (chdir(arr[1]) != 0) //successful change
			printf("ERROR %d (%s)\n", errno, strerror(errno)); //error in changing
	}
	
	else 
		forkTest(arr, inout);
		
	return loop;
}


void forkTest(char** arr, char** inout)
{
	int child_status;
	pid_t pid = fork();
	
	if (pid == 0) //child
	{
		fflush(stdout);
		
		if (strcmp(inout[0], "") != 0) //input
		{
			FILE* infile = fopen(inout[0], "r");
			dup2(fileno(infile), 0);
			fclose(infile);
		}
		
		if (strcmp(inout[1], "") != 0) //output
		{
			FILE* outfile = fopen(inout[1], "w");
			dup2(fileno(outfile), 1);
			fclose(outfile);	
		}
		
		
		execvp(arr[0], arr);
		exit(2);
	}
	
	else //parent
	{
		wait(&child_status); //wait and record the child exit status: 0=success, 2=error
		if (WEXITSTATUS(child_status) == 2) //child did not run correctly - ERROR 2
			printf("ERROR %d (%s)\n", 2, strerror(2));
	}
}

