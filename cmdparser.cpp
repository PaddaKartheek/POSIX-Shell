#include<iostream>
#include<string.h>
#include "header.h"
using namespace std;

void category(char *cmd, char *homeDir, char *prevDir, char *currDir){
    int len = strlen(cmd);
    bool pipeflag = false, inoutflag =false, andflag = false;
    for(int i=0;i<len;i++){
        if(cmd[i] == '|')
            pipeflag = true;
        else if(cmd[i] == '&')
            andflag = true;
        else if(cmd[i] == '<' || cmd[i] == '>' || (cmd[i] == '>' && cmd[i+1] != '\0' && cmd[i+1] == '>' && cmd[i+2] == '\0'))
            inoutflag = true;
    }
    if(pipeflag && andflag){
        cout << "ERROR" << endl;
    }
    else if(inoutflag && andflag){
        cout << "ERROR" << endl;
    }
    else if(pipeflag){
        pexec(cmd, homeDir, prevDir, currDir);
    }
    else if(inoutflag){
        iope(cmd, homeDir, prevDir, currDir);
    }
    else{
        executeCommand(cmd, homeDir, prevDir, currDir);
    }
}

void parse(char *input, char *homeDir, char *prevDir, char *currDir){
    char *cmds[1000];
    int i = 0;
    cmds[i] = strtok(input, ";");
    while(cmds[i]!=NULL){
        i++;
        cmds[i] = strtok(NULL,";");
    }
    int cmds_len = i;
    for(int i=0;i<cmds_len;i++){
        category(cmds[i], homeDir, prevDir, currDir);
    }
}
