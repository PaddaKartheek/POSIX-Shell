#include<iostream>
#include<string.h>
#include<termios.h>
#include<unistd.h>
#include<stdio.h>
#include<errno.h>
#include<vector>
#include<dirent.h>
#include<sys/stat.h>
#include "header.h"
#include<pwd.h>
#include<limits.h>
using namespace std;

termios term, oterm;

char getch(){
	char c;
	tcgetattr(0, &oterm);
	memcpy(&term, &oterm, sizeof(term));
	term.c_lflag &= ~(ICANON | ECHO);
	term.c_cc[VMIN] = 1;
	term.c_cc[VTIME] = 0;
	tcsetattr(0, TCSANOW, &term);
	c = getchar();
	tcsetattr(0, TCSANOW, &oterm);
	return c;
}


int autocomplete(char* input, int in){
	bool pflag = false;
	int st = in-1;
	int where = 0;
	for(int i=st;i>=0;i--){
		if(input[i] == ' ' && ((i!=0 && input[i-1] != '\\') || i == 0)){
			where = i+1;
			break;
		}
	}
	if(where == in && in!=0){
		return in;
	}
	for(int i=where;i<in;i++){
		if(input[i] == '/'){
			pflag = true;
			break;
		}
	}
	char path[1000], searchString[1000];
	if(pflag){
		int strstart = where;
		for(int i=st;i>=where;i--){
			if(input[i] == '/'){
				strstart = i+1;
				break;
			}
		}
		int k = 0;
		for(int i=strstart; i<in;i++){
			searchString[k] = input[i];
			k++;
		}
		searchString[k] = '\0';
		k = 0;
		for(int i=where;i<strstart;i++){
			path[k] = input[i];
			k++;
		}
		path[k] = '\0';
		char temp[1000];
		int j = 0;
		
		for(int i=0;i<k;i++){
			if(path[i] == '\\' && path[i+1] != '\0' && path[i+1] == ' '){
				temp[j] = ' ';
				i++;
			}
			else
				temp[j] = path[i];
			j++;
		}
		temp[j] = '\0';
		strcpy(path, temp);
	}
	else{
		getcwd(path, 1000);
		int k=0;
		for(int i=where;i<in;i++){
			searchString[k] = input[i];
			k++;
		}
		searchString[k] = '\0';
	}
	char temp[1000];
	int j = 0;
	int n = strlen(searchString);
	for(int i=0;i<n;i++){
		if(searchString[i] == '\\' && searchString[i+1] != '\0' && searchString[i+1] == ' ' ){
			temp[j] = ' ';
			i++;
		}
		else
			temp[j] = searchString[i];
		j++;
	}
	temp[j] = '\0';
	strcpy(searchString, temp);
	struct stat path_stat;
	if(stat(path, &path_stat)!=0){
		int error_number = errno;
        	printf("Error message: %s (%s)\n", strerror(error_number), path);
        	return in;
	}
	DIR *dir = opendir(path);
        if (dir == NULL) {
            printf("Error message: %s (%s)\n", strerror(errno), path);
            return in;
        }
        struct dirent *entry;
        vector<char*> matches;
        int len = strlen(searchString);
        while ((entry = readdir(dir)) != NULL) {
                if(strncmp(searchString, entry->d_name, len) == 0 && len < strlen(entry->d_name))
                	matches.push_back(entry->d_name);        
        }
    closedir(dir);   
	if(matches.size() == 1){
		char remString[1000];
		int newlen = strlen(matches[0]);
		int ti = 0;
		for(int i=len;i<newlen;i++){
			if(matches[0][i] == ' '){
				remString[ti] = '\\';
				ti++;
			}
			remString[ti] = matches[0][i];
			ti++;
		}
		remString[ti] = '\0';
		int l = strlen(remString);
		for(int i=0;i<l;i++){
			input[in] = remString[i];
			in++;
		}
		input[in] = '\0';
		return in;
	}
	else if(matches.size() > 1){
		cout << endl;
		for(int i=0;i<matches.size();i++){
			cout << matches[i] << " ";
		}
		cout << endl;
		return in;
	}
	else 
		return in;
}


int main() {
	char input[1000];
	char ch;
	int i=0;
    char hostname[HOST_NAME_MAX];
    char homedir[PATH_MAX];
    char prevdir[PATH_MAX];
    char currdir[PATH_MAX];

    // Get the hostname
    if(gethostname(hostname, HOST_NAME_MAX) == -1)
        cout << "Error while getting the host name" << endl;
    

    // Get the username
    char *username = getlogin();
    if(username==nullptr)
        cout << "Error while getting the username" << endl;
    
    
    if(getcwd(homedir, PATH_MAX) == NULL)
        cout << "Error while getting the current working directory" << endl;
    
    strcpy(currdir, homedir);
    prevdir[0] = '\0';
    char disname[1000];
	char display[10000];
    while(1){
        i = 0;
        if(strncmp(currdir, homedir, strlen(homedir)) == 0){
            int ani = strlen(homedir);
            disname[0] = '~';
            int k = 1;
            for(int m=ani;m<strlen(currdir);m++){
                disname[k] = currdir[m];
                k++;
            }
            disname[k] = '\0';
        }
        else
            strcpy(disname, currdir);
        sprintf(display, "%s@%s:%s>", username, hostname, disname);
        cout << display;
        do{
            ch = getch();
            if(ch == '\t'){
                i = autocomplete(input, i);
            }
            else if(i!=0 && (int)(ch) == 127){
                cout << '\r' << display;
                for(int j=0;j<i;j++)
                    cout << ' ';
                i--;
            }
			else if(ch == '\x04'){
				cout << endl;
				exit(0);
			}
            else if(ch!= '\x1B' &&ch!='\x04' && ch != '\x03' && ch != '\x1A' && ch != '\n' && (int)(ch) != 127){
                input[i] = ch;
                i++;
            }
            cout << '\r' << display;
            for(int j=0;j<i;j++)
                cout << input[j];
        }while(ch!='\n');
        cout << endl;
        input[i] = '\0';
		parse(input, homedir, prevdir, currdir);
    }
}