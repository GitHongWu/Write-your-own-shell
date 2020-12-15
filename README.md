# Write-your-own-shell

### Background
One of the fundamental functionalities of Linux (and Linux-like OSs) is the ability to pipe commands. When the | character is used in a shell between two commands, the shell takes the output from the first command and “pipes” it to the second command as the input. You can try it yourself! If you run the command ls -la / | grep tmp on systems1, you should be able to see only the tmp directory output from ls -la. This is because you gave grep, a common search tool, the output of ls -la directly. There are many more applications of piping but that is for you to discover as necessary in your adventures/projects/jobs/etc etc etc.

### The programming part
For this assignment you are to write a simple program that will act as a shell. The program should:

* Display a command prompt and read in a command line from the user (the prompt must be CS361 >, otherwise it cannot be detected by the test script)
* Your shell must support basic piping like the unix shell.
  1. command Run command, with stdin and stdout connected to their usual files. When command finishes, the parent should output pid:%d status:%d\n (with the proper relevant values inserted into the format string), and then print the prompt again and wait for more input.
  2. command1 | command2 Run command1 as in #1, but redirects the output of command1 as input to command2. Note that there should be two instances of pid:%d status:%d\n output.
  3. command1 ; command2 Run command1 as in #1, wait for it to finish, and then run command 2 as in #1.

Also…

* Your shell should handle the following signals:
  * SIGINT - Generated by Ctrl-C. This signal allows a user to terminate a running program. Your shell should not exit when user presses Ctrl-C or the process receives SIGINT but simply report that the SIGINT signal has been received by the shell. If the process receives SIGINT, you must print the string “caught sigint” on a line by itself, and then show the prompt again.
  * SIGTSTP - Generated by Ctrl-Z. This signal allows a user to terminate a running program. Your shell should not exit when user presses Ctrl-Z or the process receives SIGTSTP but simply report that the SIGTSTP signal has been received by the shell. If your shell receives SIGTSTP, you must print the string “caught sigstp” on a line by itself, and then show the prompt again.
* The shell does NOT need to support background processes, running more than one child at a time, or handling multiple chained pipes. Essentially, if anything not matching the use cases/requirements above is in question, you probably do not have to do it. Due to the breadth of this assignment, it is difficult to clarify every case in writing. Please ask in office hours or on Piazza if you have questions or need clarification.

### Tips for primary components
* Parse the command line into arguments, creating an array of character pointers, where array[0] points to the actual command and rest of the array elements point to the arguments to the command (Similar to main()’s argv[])
* Fork off a child and have the child load the requested program by passing the argument vector created in step 2 to exec() family of system calls. The parent should report the PID of the child before proceeding to the next step. (report means you are required to return strings contain “pid” and “status”, e.g. pid:11111 status:0)
* Wait for the child to complete executing and report why it ended (exited or uncaught signal) and its exit value if available.
* Repeat the first step forever until the user enters the command exit

### Requirements
See above, but here is a summary:

  1. Display a command prompt (CS361 >) and read in a command line from the user
  2. Handle $ command (run command, with stdin and stdout connected to their usual files, must additionally output pid:%d status:%d\n, then continue)
  3. Handle $ command1 | command2 (run command1 as in #1, but redirects the output of command1 as input to command2, then continue)
  4. Handle $ command1 ; command2 (run command1 as in #1, wait for it to finish, and then run command 2 as in #1, then continue)
  5. Handle SIGINT - Generated by Ctrl-C (print “caught sigint” and then continue)
  6. Handle SIGTSTP - Generated by Ctrl-Z (print “caught sigstp” and then continue)
It is highly recommended you develop and test your code on systems1 or a similar Linux environment. This is simply to ensure standardization of comparisons between our and your tests whilst you are developing your code.

### Command
gcc -Wall -Werror -o hw3 hw3.c
