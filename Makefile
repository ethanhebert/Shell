all: techshell

techshell: techshell.c
	gcc -o techshell techshell.c

clean:
	rm -f techshell
