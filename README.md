## UID: 605796227

## Pipe Up
For this lab we create a program that runs command line arguments and replicates the pipe function operating through the opening, closing and redirecting of file descripttors.

## Building

To build this program, you need to execute the makefile in the directory by running it in terminal. 
Open terminal, navigate to the directory where the files are located, and then run the command: make
this would run the makefile with the specific Cflags and FDflags, creating another executable file pipe.

## Running
to run this executable file pipe, we write the command ./pipe [command1] [command2] .....[command n], and run this in the terminal. if there are any errors with the piping, or syntax errors, it would send errors and would write that to the terminal.
for instance, the command : 
./pipe ls sort 
will list all of the files and folders in the current directory in a sorted manner
## Cleaning up

Explain briefly how to clean up all binary files
