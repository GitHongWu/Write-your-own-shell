
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CMD 500

void sigtstp_handler(int sig){
    char msg[] = "caught sigtstp\n";
    write(1, msg, sizeof(msg));
    return;
}

void sigint_handler(int sig){
    char msg[] = "caught sigint\n";
    write(1, msg, sizeof(msg));
    return;
}

//debug display the input array
void printCommandArray(char* argsarray[], int size){
    int j=0;
    for (;j<size;j++){
        printf("argsarray[%d]: %s\n", j, argsarray[j]);
    }
    printf("argsarray[%d]: %s\n", j, argsarray[j]);
}

void parseCommand(char* argsarray[], int size, char* c1[], char* c2[]){
    int flag = 0;
    int j = 0;
    int index = 0;
    for (;j<size;j++){
        if(strcmp(argsarray[j], "|") == 0){
            flag = 2;
            index= j;
            break;
        }
        else if(strcmp(argsarray[j], ";") == 0){
            flag = 3;
            index= j;
            break;
        }
        else{
            flag = 1;
        }
    }

    //split command to two
    if(flag == 2 || flag == 3){
        int i;
        for(i = 0; i < index; i++){
            c1[i] = argsarray[i];
        }
        c1[i] = NULL;

        for(j = index + 1; j < size; j++){
            c2[j - index -1] = argsarray[j];
        }
        c2[j - index - 1] = NULL;
    }

    //execute '|' command
    if(flag == 2){
        int fd[2];  
        int pid1;
        int pid2;
        pipe(fd);
        pid1 = fork();
        if (pid1 == 0) {
            close(fd[0]); 
            dup2(fd[1], 1); //dup from fd1 to output
            close(fd[1]); 
            execvp(c1[0], c1);
        } else { 
            pid2 = fork();
            int status2;
            if (pid2 == 0) { 
                close(fd[1]); 
                dup2(fd[0], 0); //dup from fd0 to input
                close(fd[0]); 
                execvp(c2[0], c2);
            } else { 
                close(fd[0]);
                close(fd[1]);
                wait(&status2);
                
            }
            int status1;
            wait(&status1);
            printf("pid:%d status:%d\n", pid1, WEXITSTATUS(status1));
            printf("pid:%d status:%d\n", pid2, WEXITSTATUS(status2));
        }
    }
    //execute ';' command
    else if(flag == 3){
        int pid = fork();
        if(pid == 0) { // child
            execvp(c1[0], c1);
        } else { // parent
            int status;
            wait(&status);
            int pid2 = fork();
            if(pid2 == 0) {
                execvp(c2[0], c2);
            } else {
                wait(&status);
                printf("pid:%d status:%d\n", pid2, WEXITSTATUS(status));
            }
            printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
        }
    }
    return;
}

int main(){

    signal(SIGINT, sigint_handler);
    signal(SIGTSTP, sigtstp_handler);

    while(1){
        char line[CMD];
        //allcoate memary
        char* argsarray[20];
        int i;
        for(i = 0; i < 20; i++){
            argsarray[i] = (char* )malloc(100 * sizeof(char));
        }

        printf("CS361 >");
        //get lab
        fgets(line, CMD, stdin);
        //if input is exit
        if(strcmp(line,"exit\n") == 0){
            exit(0);
        }
        //replace the last char to \0
        line[strlen(line) - 1] = '\0';

        char *word = strtok(line, " ");
        i = 0;
        while(word)
        {
            strcpy(argsarray[i], word);
            word = strtok(NULL, " ");
            i = i + 1;
        }
        argsarray[i]=NULL;
        int counter = i;

        //debug
        //printCommandArray(argsarray, counter);

        char* c1[20];
        char* c2[20];
        c1[0] = NULL;   //init c1 and c2
        c2[0] = NULL;
        parseCommand(argsarray, counter, c1, c2);

        if(c1[0] != NULL){
            //printf("Command contain '|' or ';'\n");

            //command contain '|' and ';'
            //do nothing, command has executed in parseCommand
        }
        else{
            int pid = fork();
            if (pid == 0) {
                execvp(argsarray[0], argsarray);
                exit(0);
            } else {
                int status;
                wait(&status);
                printf("pid:%d status:%d\n", pid, WEXITSTATUS(status));
            }
        }
        
        for(i=0;i<20;i++){
            if(argsarray[i] == NULL){
                free (argsarray[i]);
            }
        }
    }

    return 0;
}