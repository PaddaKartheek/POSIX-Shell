#include<iostream>
#include<string.h>
#include "header.h"
using namespace std;


void echo(char *cmd){
    int len = 0;
    while(cmd[len] != '\0'){
        len++;
    }
    char *output;
    output = strtok(cmd, "\t ");
    int in = 0;
    while(cmd[in] != '\0'){
        in++;
    }
    in++;
    int k=0;
    char temp[1000];
    for(int i=in;i<len;i++){
        temp[k] = cmd[i];
        k++;
    }
    temp[k] = '\0';
    char *portion = strtok(temp, "\t ");
    while(portion != NULL){
        cout << portion << " ";
        portion = strtok(NULL, "\t ");
    }
    cout << endl;
}
