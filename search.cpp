#include<iostream>
#include<string.h>
#include<queue>
#include<errno.h>
#include<unistd.h>
#include<dirent.h>
#include<sys/stat.h>
#include "header.h"
using namespace std;

string charToString(char *str){
    string dum = "";
    int i=0;
    while(str[i] != '\0'){
        dum = dum + str[i];
        i++;
    }
    return dum;
}
void search(char *cmd, char *currDir){
    int i=0;
    while(cmd[i] != '\0'){
        if(cmd[i] == '\\' && cmd[i+1] != '\0' && cmd[i+1] == ' '){
            cmd[i+1] = '\n';
            i++;
        }
        i++;
    }
    char *searchFile;
    const char delimiters[] = "\t ";
    searchFile = strtok(cmd, delimiters);
    searchFile = strtok(NULL, delimiters);
    if(searchFile == NULL){
        cout << "No argument given" << endl;
        return;
    }
    else if(searchFile != NULL && strtok(NULL, delimiters) != NULL){
        cout << "Too many arguments" << endl;
        return;
    }
    i = 0;
    char temp[1000];
    int k = 0;
    while(searchFile[i] != '\0'){
        if(searchFile[i] == '\\' && searchFile[i+1] != '\0' && searchFile[i+1] == '\n'){
            temp[k] = ' ';
            i++;
        }
        else 
            temp[k] = searchFile[i];
        k++;
        i++;
    }
    temp[k] = '\0';
    strcpy(searchFile, temp);
    queue<string> searchQueue; 
    searchQueue.push(charToString(currDir));
    bool found = false;
    while(!searchQueue.empty()){
        string path = searchQueue.front();
        searchQueue.pop();
        DIR *dir = opendir(path.c_str());
        if(dir == NULL)
            continue;
        struct dirent *entry;
        while((entry = readdir(dir)) != NULL){
            string andum = "";
            if(entry->d_name[0] == '.' && (entry->d_name[1] == '\0' || (entry->d_name[1] == '.' && entry->d_name[2] == '\0')))
                continue;
            if(strcmp(searchFile, entry->d_name) == 0){
                found = true;
                break;
            }
            
            andum = andum + path;
            if(path[0]=='/' && path[1] != '\0')
                andum = andum + "/";
            andum = andum + charToString(entry->d_name);
            struct stat path_stat;
            if(stat(andum.c_str(), &path_stat) != 0 && lstat(andum.c_str(), &path_stat) != 0){
                int error_number = errno;
                printf("Error message: %s (%s)\n", strerror(error_number), andum.c_str());
                return;
            }
            if(S_ISDIR(path_stat.st_mode)){
                searchQueue.push(andum);
            }
        }
        if(found)
            break;
    }
    if(found)
        cout << "True" << endl;
    else    
        cout << "False" << endl;
}