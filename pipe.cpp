#include<iostream>
#include<string.h>
#include<fcntl.h>
#include "header.h"
#include<unistd.h>
#include<errno.h>
#include<sys/wait.h>
using namespace std;

bool check(char *text){
    int n = strlen(text);
    for(int i=0;i<n;i++){
        if(text[i] == '<' || text[i] == '>'){
            return true;
        }
    }
    return false;
}

void pexec(char *cmd, char *homeDir, char *prevDir, char *currDir){
    if(cmd[0] == '|' || cmd[strlen(cmd)-1] == '|'){
        cout << "Invalid command" << endl;
        return;
    }
    char *tokenString[1000];
    int i = 0;
    tokenString[i] = strtok(cmd, "|");
    while(tokenString[i] != NULL){
        i++;
        tokenString[i] = strtok(NULL, "|");
    }
    int tk_len = i;
    int pipe_ope[tk_len-1][2];
    for(int j=0;j<(tk_len-1);j++){
        if(pipe(pipe_ope[j]) == -1){
  			cout << strerror(errno) << endl;
  		}
    }

    for(int j=0;j<tk_len;j++){
        int pid = fork();
        if(pid == 0){
            if(j!=0){
                if(dup2(pipe_ope[j-1][0], STDIN_FILENO) == -1)
                    cout << strerror(errno) << endl;
            }
            if(j!=tk_len-1){
                if(dup2(pipe_ope[j][1], STDOUT_FILENO) == -1)
                    cout << strerror(errno) << endl;
            }
            if(check(tokenString[j])){
                iope(tokenString[j], homeDir, prevDir, currDir);
            }else{
                executeCommand(tokenString[j], homeDir, prevDir, currDir);
            }
            exit(0);
        }else{
            if(j != tk_len-1)
                close(pipe_ope[j][1]);
            if(j != 0)
                close(pipe_ope[j-1][0]);
            wait(NULL);
        }
    }
}