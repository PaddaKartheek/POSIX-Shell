#include<iostream>
#include "header.h"
#include<string.h>
#include<unistd.h>
#include<filesystem>
using namespace std;

void cd(char* homeDir, char* prevDir, char* currDir, char* cmd){
    char *path;
    char temp[1000];
    const char delimiters[] = "\t ";
    int cn = 1;
    int i = 0;
    while(cmd[i] != '\0'){
        if(cmd[i] == '\\' && cmd[i+1] != '\0' && cmd[i+1] == ' '){
            cmd[i+1] = '\n';
            i++;
        }
        i++;
    }
    path = strtok(cmd, delimiters);
    path = strtok(NULL, delimiters);
    if(path != NULL && strtok(NULL, delimiters) != NULL){
        cout << "Too many arguments" << endl;
        return;
    }
    int k = 0;
    i = 0;
    if(path != NULL){
        while(path[i] != '\0'){
            if(path[i] == '\\' && path[i+1] != '\0' && path[i+1] == '\n'){
                temp[k] = ' ';
                i++;
            }
            else
                temp[k] = path[i];
            i++;
            k++;
        }
        temp[k] = '\0';
        strcpy(path, temp);
    }
    if(path == NULL){
        strcpy(prevDir, currDir);
        strcpy(currDir, homeDir);
        chdir(homeDir);
    }
    else if(path[0] == '-' && path[1] == '\0'){
        if(prevDir[0] == '\0'){
            cout << "OLDPWD is not set" << endl;
            return;
        }else{
            char temp[sizeof(prevDir)];
            strcpy(temp, prevDir);
            strcpy(prevDir,currDir);
            strcpy(currDir,temp);
            chdir(currDir);
            cout << currDir << endl;
        }
    }
    else if(path[0] == '~' && path[1] == '\0'){
        strcpy(prevDir, currDir);
        strcpy(currDir, homeDir);
        chdir(homeDir);
    }
    else if(path[0] == '.' && path[1] == '\0'){
        if(prevDir[0] == '\0'){
            strcpy(prevDir, currDir);
        }
    }
    else if(path[0] == '.' && path[1] == '.'){
        if(chdir(path) == -1){
            cout << "Error changing to directory" << endl;
        }
        getcwd(temp, 1000);
        strcpy(prevDir, currDir);
        strcpy(currDir, temp);
    }
    else if(filesystem::exists(path)){
        chdir(path);
        strcpy(prevDir, currDir);
        getcwd(temp, 1000);
        strcpy(currDir, temp);
    }
    else{
        cout << path << ": No such file or directory" << endl;
    }
}