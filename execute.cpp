#include "header.h"
#include<string.h>
#include<iostream>
using namespace std;

void executeCommand(char *cmd, char* homeDir, char* prevDir, char* currDir){
    char temp[1000];
    strcpy(temp, cmd);
    char *cmdportion = strtok(temp, "\t ");
    char text[] = "search";
    char ptext[] = "pinfo";
    if(cmdportion == NULL){
        return ;
    }
    else if(cmdportion[0] == 'c' && cmdportion[1] != '\0' && cmdportion[1] == 'd' && cmdportion[2] == '\0'){
        cd(homeDir, prevDir, currDir, cmd);
    }
    else if(cmdportion[0] == 'l' && cmdportion[1] != '\0' && cmdportion[1] == 's' && cmdportion[2] == '\0'){
        lspe(cmd, homeDir, currDir);
    }
    else if(cmdportion[0] == 'e' && cmdportion[1] != '\0' && cmdportion[1] == 'c' && cmdportion[2] != '\0' && cmdportion[2] == 'h' && cmdportion[3] != '\0' && cmdportion[3] == 'o' && cmdportion[4] == '\0'){
        echo(cmd);
    }
    else if(cmdportion[0] == 'p' && cmdportion[1] != '\0' && cmdportion[1] == 'w' && cmdportion[2] != '\0' && cmdportion[2] == 'd' && cmdportion[3] == '\0'){
        cout << currDir << endl;
    }
    else if(strcmp(cmdportion, text) == 0){
        search(cmd, currDir);
    }
    else if(strcmp(cmdportion, ptext) == 0){
        ppinfo(cmd);
    }
    else{
        esyscom(cmd);
    }
}