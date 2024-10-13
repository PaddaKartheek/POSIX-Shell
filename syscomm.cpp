#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include "header.h"
using namespace std;

void esyscom(char* cmd){
    int i = 0;
    while(cmd[i] != '\0'){
        if(cmd[i] == '\\' && cmd[i+1] != '\0' && cmd[i+1] == ' '){
            cmd[i+1] = '\n';
            i++;
        }
        i++;
    }
    char *args[1000];
    args[0] = strtok(cmd, "\t ");
    i = 0;
    while(args[i] != NULL){
        i++;
        args[i] = strtok(NULL, "\t ");
    }
    int args_len = i;
    
    char temp[1000];
    for(int m=0;m<args_len;m++){
        int j=0;
        int k=0;
        while(args[m][j] != '\0'){
            if(args[m][j] == '\\' && args[m][j+1] != '\0' && args[m][j+1] == '\n'){
                temp[k] = ' ';
                j++;
            }
            else
                temp[k] = args[m][j];
            j++;
            k++;
        }
        temp[k] = '\0';
        strcpy(args[m], temp);
    }
    int pid = fork();
    if(pid == 0){
        int res = execvp(args[0], args);
        if(res < 0){
            cout << args[0] << ": Invalid command" << endl;
            exit(0);
        }
    }else{
        int status;
        wait(&status);
    }
}

