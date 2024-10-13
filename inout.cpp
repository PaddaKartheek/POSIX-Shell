#include<iostream>
#include "header.h"
#include<string.h>
#include<fcntl.h>
#include<unistd.h>
#include<vector>
#include<sys/wait.h>
using namespace std;

void ioe(char *tokenString[], int tk_len, bool iflag, bool oflag, bool appendflag, char* homeDir, char* prevDir, char* currDir){
    int original = dup(1);
    int fd = -1;
    if(oflag || appendflag){
        int in = (tk_len == 3)?2:4;
        if(oflag)
            fd = open(tokenString[in], O_WRONLY | O_TRUNC | O_CREAT, 0644);
        else
            fd = open(tokenString[in], O_WRONLY | O_APPEND | O_CREAT, 0644);
        dup2(fd, 1);
        if(tk_len == 3){
            executeCommand(tokenString[0], homeDir, prevDir, currDir);
        }
    }
    if(iflag){
        int fd2 = open(tokenString[2], O_RDONLY);
        int pid = fork();
        if(pid == 0){
            int org = dup(0);
            tokenString[1] = NULL;
            dup2(fd2, 0);
            executeCommand(tokenString[0], homeDir, prevDir, currDir);
            dup2(org, 0);
            exit(0);
        }
        else{
            wait(NULL);
            close(fd2);
        }
    }
    dup2(original, 1);
    if(fd != -1)
        close(fd);
}


int combineFirstargument(char *tokenString[], int tk_len){
    char h[1000];
    int i;
    for(i=0;i<tk_len;i++){
        if(tokenString[i][0] == '<' || tokenString[i][0] == '>'){
            break;
        }
    }
    int l = -1;
    for(int j=0;j<i;j++){
        l++;
        int n = strlen(tokenString[j]);
        for(int k=0;k<n;k++){
            h[l] = tokenString[j][k];
            l++;
        }
        h[l] = ' ';
    }
    h[l] = '\0';
    int newIn = 1;
    tokenString[0] = h;
    for(int j=i;j<tk_len;j++){
        tokenString[newIn] = tokenString[j];
        newIn++;
    }
    tokenString[newIn] = NULL;
    return newIn;
}

void iope(char *cmd, char* homeDir, char* prevDir, char* currDir){
    int i=0;
    while(cmd[i] != '\0'){
        if(cmd[i] == '\\' && cmd[i+1] != '\0' && cmd[i+1] == ' '){
            cmd[i+1] = '\n';
            i++;
        }
        i++;
    }
    char *tokenString[1000];
    i = 0;
    tokenString[i] = strtok(cmd, "\t ");
    while(tokenString[i] != NULL){
        i++;
        tokenString[i] = strtok(NULL, "\t ");
    }
    int tk_len = i;
    char temp[1000];
    for(int m=0;m<tk_len;m++){
        int j=0;
        int k=0;
        while(tokenString[m][j] != '\0'){
            if(tokenString[m][j] == '\\' && tokenString[m][j+1] != '\0' && tokenString[m][j+1] == '\n'){
                temp[k] = ' ';
                j++;
            }
            else   
                temp[k] = tokenString[m][j];
            k++;
            j++;
        }
        temp[k] = '\0';
        strcpy(tokenString[m], temp);
    }
    tk_len = combineFirstargument(tokenString, tk_len);
    if(tk_len == 3){
        if(tokenString[1][0] == '>' && tokenString[1][1] == '\0')
            ioe(tokenString, tk_len, false, true, false, homeDir, prevDir, currDir);
        else if(tokenString[1][0] == '<' && tokenString[1][1] == '\0')
            ioe(tokenString, tk_len, true, false, false, homeDir, prevDir, currDir);
        else if(tokenString[1][0] == '>' && tokenString[1][1] != '\0' && tokenString[1][1] == '>' && tokenString[1][2] == '\0')
            ioe(tokenString, tk_len, false, false, true, homeDir, prevDir, currDir);
    }
    else if(tk_len == 5){
        if(tokenString[1][0] == '<' && tokenString[1][1] == '\0' && tokenString[3][0] == '>' && tokenString[3][1] == '\0')
            ioe(tokenString, tk_len, true, true, false, homeDir, prevDir, currDir);
        else if(tokenString[1][0] == '<' && tokenString[1][1] == '\0' && tokenString[3][0] == '>' && tokenString[3][1] != '\0' && tokenString[3][1] == '>' && tokenString[3][2] == '\0')
            ioe(tokenString, tk_len, true, false, true, homeDir, prevDir, currDir);
    }
    else{
        
        return;
    }
}



 // int l1 = strlen(tokenString[0]), l2 = strlen(tokenString[2]);
        // int l = l1 + l2 + 2;
        // char input[l];
        // for(int i=0;i<l1;i++)
        //     input[i] = tokenString[0][i];
        // input[l1] = ' ';
        // for(int i=0;i<l2;i++)
        //     input[i+l1+1] = tokenString[2][i];
        // input[l-1] = '\0';
        // char *argv[7];
        // char exe[] = "./temporary";
        // char path[1000],st[3] = "/";
        // strcpy(path, homeDir);
        // strcat(path, st);
        // strcat(path, exe);
        // argv[0] = path;
        // argv[1] = input;
        // argv[2] = tokenString[2];
        // argv[3] = homeDir;
        // argv[4] = prevDir;
        // argv[5] = currDir;
        // argv[6] = NULL;
        // int pid = fork();
        // if(pid == 0){
        //     int res = execvp(argv[0], argv);
        //     if(res < 0){
        //         exit(0);
        //     }
        // }
        // else{
        //     wait(NULL);
        // }