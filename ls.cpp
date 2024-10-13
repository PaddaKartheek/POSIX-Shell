#include<iostream>
#include<string.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<errno.h>
#include<stdio.h>
#include<time.h>
#include<grp.h>
#include<pwd.h>
#include<dirent.h>
#include<unistd.h>
#include "header.h"
using namespace std;


void printSymbolicLink(char *path, char *name, struct stat path_stat){
    char targetPath[996];
    readlink(path, targetPath, sizeof(targetPath)-1);
    char newName[1000];
    sprintf(newName,"%s -> %s", name, targetPath);
    char buff[2000];
    sprintf(buff, "l%c%c%c%c%c%c%c%c%c %ld %s %s %lld %s %s", 
    (path_stat.st_mode&S_IRUSR)?'r':'-',
    (path_stat.st_mode&S_IWUSR)?'w':'-', 
    (path_stat.st_mode&S_IXUSR)?'x':'-', 
    (path_stat.st_mode&S_IRGRP)?'r':'-', 
    (path_stat.st_mode&S_IWGRP)?'w':'-', 
    (path_stat.st_mode&S_IXGRP)?'x':'-', 
    (path_stat.st_mode&S_IROTH)?'r':'-', 
    (path_stat.st_mode&S_IWOTH)?'w':'-', 
    (path_stat.st_mode&S_IXOTH)?'x':'-',
    (long)path_stat.st_nlink,
    getpwuid(path_stat.st_uid)->pw_name,
    getgrgid(path_stat.st_uid)->gr_name,
    (long long)path_stat.st_size,
    strtok(ctime(&path_stat.st_mtime), "\n"),
    newName);
    cout << buff << endl;
}

void list(char* path, bool lflag, bool aflag){
    struct stat path_stat;
    if(stat(path, &path_stat) != 0){
        int error_number = errno;
        printf("Error message: %s (%s)\n", strerror(error_number), path);
        return;
    }
    if(S_ISREG(path_stat.st_mode)){
        if(!lflag){
            cout << path << endl;
        }
        else{
            char buff[1000];
            sprintf(buff, "-%c%c%c%c%c%c%c%c%c %ld %s %s %lld %s %s", 
             (path_stat.st_mode&S_IRUSR)?'r':'-',
             (path_stat.st_mode&S_IWUSR)?'w':'-', 
             (path_stat.st_mode&S_IXUSR)?'x':'-', 
             (path_stat.st_mode&S_IRGRP)?'r':'-', 
             (path_stat.st_mode&S_IWGRP)?'w':'-', 
             (path_stat.st_mode&S_IXGRP)?'x':'-', 
             (path_stat.st_mode&S_IROTH)?'r':'-', 
             (path_stat.st_mode&S_IWOTH)?'w':'-', 
             (path_stat.st_mode&S_IXOTH)?'x':'-',
             (long)path_stat.st_nlink,
             getpwuid(path_stat.st_uid)->pw_name,
             getgrgid(path_stat.st_uid)->gr_name,
             (long long)path_stat.st_size,
             strtok(ctime(&path_stat.st_mtime), "\n"),
             path);
            cout << buff << endl;
        }
    }
    else if(S_ISDIR(path_stat.st_mode)){
        DIR *dir = opendir(path);
        if (dir == NULL) {
            printf("Error message: %s (%s)\n", strerror(errno), path);
            return;
        }
        cout << endl << path << ":" << endl;
        struct dirent *entry;
        if(!lflag){
            while ((entry = readdir(dir)) != NULL) {
                if(entry->d_name[0]=='.' && !aflag)
                    continue;     
                cout << entry->d_name << endl;
            }
        }else{
            char newPath[1000];
            char buff[1000];
            while((entry = readdir(dir)) != NULL){
                if(entry->d_name[0]=='.' && !aflag)
                    continue; 
                sprintf(newPath,"%s/%s", path, entry->d_name);
                struct stat newPath_stat;
                if(stat(newPath, &newPath_stat)!=0){
                    if(lstat(newPath, &newPath_stat)==0){
                        printSymbolicLink(newPath, entry->d_name, newPath_stat);
                        continue;
                    }
                    else{
                        int error_number = errno;
                        printf("Error message: %s (%s)\n", strerror(error_number), newPath);
                        return;
                    }
                }
                sprintf(buff, "%c%c%c%c%c%c%c%c%c%c %ld %s %s %lld %s %s", 
                S_ISDIR(newPath_stat.st_mode)?'d':'-',
                (newPath_stat.st_mode&S_IRUSR)?'r':'-',
                (newPath_stat.st_mode&S_IWUSR)?'w':'-', 
                (newPath_stat.st_mode&S_IXUSR)?'x':'-', 
                (newPath_stat.st_mode&S_IRGRP)?'r':'-', 
                (newPath_stat.st_mode&S_IWGRP)?'w':'-', 
                (newPath_stat.st_mode&S_IXGRP)?'x':'-', 
                (newPath_stat.st_mode&S_IROTH)?'r':'-', 
                (newPath_stat.st_mode&S_IWOTH)?'w':'-', 
                (newPath_stat.st_mode&S_IXOTH)?'x':'-',
                (long)newPath_stat.st_nlink,
                getpwuid(newPath_stat.st_uid)->pw_name,
                getgrgid(newPath_stat.st_uid)->gr_name,
                (long long)newPath_stat.st_size,
                strtok(ctime(&newPath_stat.st_mtime), "\n"),
                entry->d_name);
                cout << buff << endl;
            }
        }
        closedir(dir);
    }
}

void lspe(char* cmd, char* homeDir, char* currDir){
    char *tokenString[1000];
    bool aflag = false, lflag = false;
    int i = 0;
    while(cmd[i] != '\0'){
        if(cmd[i] == '\\' && cmd[i+1] != '\0' && cmd[i+1] == ' '){
            cmd[i+1] = '\n';
            i++;
        }
        i++;
    }
    i = 0;
    const char delimiters[] = "\t ";
    tokenString[i] = strtok(cmd, delimiters);
    while(tokenString[i] != NULL){
        i++;
        tokenString[i] = strtok(NULL, delimiters);
    }
    int ts_len = i;
    char temp[1000];
    for(int m=1;m<ts_len;m++){
        int j = 0;
        int k = 0;
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
    for(int m=1;m<ts_len;m++){
        if(tokenString[m][0] == '-' && tokenString[m][1] != '\0'){
            if(tokenString[m][1] == 'l' && tokenString[m][2] == '\0'){
                lflag = true;
            }
            else if(tokenString[m][1] == 'a' && tokenString[m][2] == '\0'){
                aflag = true;
            }
            else if(tokenString[m][1] == 'l' && tokenString[m][2] == 'a' && tokenString[m][3] == '\0'){
                lflag = true;aflag = true;
            }
            else if(tokenString[m][1] == 'a' && tokenString[m][2] == 'l' && tokenString[m][3] == '\0'){
                lflag = true;aflag = true;
            }
        }
    }
    char path[1000][1000];
    int k=0;
    for(int m=1;m<ts_len;m++){
        if(tokenString[m][0] != '-'){
            strcpy(path[k], tokenString[m]);
            k++;
        }
    }
    int paths_len = k;
    for(int m=0;m<paths_len;m++){
        if(path[m][0] == '~' && path[m][1] == '\0')
            list(homeDir, lflag, aflag);
        else
            list(path[m], lflag, aflag);
    }
    if(paths_len == 0){
        list(currDir, lflag, aflag);
    }
}
