#ifndef HEADER_H
#define HEADER_H
void cd(char* home, char* prev, char* curr, char* cmd);
void lspe(char* cmd, char* homeDir, char* currDir);
void echo(char *cmd);
void esyscom(char* cmd);
void executeCommand(char *cmd, char* homeDir, char* prevDir, char* currDir);
void iope(char *cmd, char* homeDir, char* prevDir, char* currDir);
void parse(char *input, char *homeDir, char *prevDir, char *currDir);
void pexec(char *cmd, char *homeDir, char *prevDir, char *currDir);
void search(char *cmd, char *currDir);
void ppinfo(char *cmd);
#endif