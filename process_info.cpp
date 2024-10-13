#include "header.h"
#include<string.h>
#include<iostream>
#include<fcntl.h>
#include<unistd.h>
using namespace std;


void pinfo(int process_id){
	char proc[] = "/proc/";
	char stat[] = "/stat";
	char cmdline[] = "/cmdline";
	char temp[1000];
	strcpy(temp, proc);
	strcat(temp, to_string(process_id).c_str());
	strcat(temp, stat);
	char buff[1000];
	int fd = open(temp, O_RDONLY);
	if(fd == -1){
		cout << "Process does not exist" << endl;
		return;
	}
	ssize_t bytesRead;
	char process_stat[10000];
	int k = 0;
	while((bytesRead = read(fd, buff, 1000)) > 0){
		for(int i=0;i<bytesRead;i++){
			process_stat[k] = buff[i];
			k++;
		}
	}
	close(fd);
	process_stat[k] = '\0';
	char *tokenString[1000];
	k = 0;
	tokenString[k] = strtok(process_stat, " ");
	while(tokenString[k] != NULL){
		k++;
		tokenString[k] = strtok(NULL, " ");
	}
	strcpy(temp, proc);
	strcat(temp, to_string(process_id).c_str());
	strcat(temp, cmdline);
	fd = open(temp, O_RDONLY);
	char process_cmd[1000];
	k = 0;
	while((bytesRead = read(fd, buff, 1000)) > 0){
		for(int i=0;i<bytesRead;i++){
			process_cmd[k] = buff[i];
			k++;
		}
	}
	process_cmd[k] = '\0';
	char *cmdpath = strtok(process_cmd, " ");
	cout << "pid -- " << tokenString[0] << endl;
	cout << "Process Status -- " << tokenString[2] << endl;
	cout << "memory -- " << tokenString[22] << endl;
	cout << "Executable Path -- " << cmdpath << endl;
}

void ppinfo(char *cmd){
    char *tokenString = strtok(cmd, " ");
    tokenString = strtok(NULL, " ");
    if(tokenString == NULL){
        cout << "No arguments" << endl;
        return;
    }
    if(strtok(NULL, " ") != NULL){
        cout << "Too many arguments" << endl;
        return;
    }
    int num = 0;
    int len = strlen(tokenString);
    for(int i=0;i<len;i++){
        if((int)(tokenString[i]) < (int)('0') || (int)(tokenString[i]) > (int)('9')){
            cout << "Not a Number" << endl;
            return;
        }
        num = (num*10) + (tokenString[i] - '0');
    }
    pinfo(num);
}