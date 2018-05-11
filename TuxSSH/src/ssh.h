#include <stdlib.h>

int checkProfiles(int profileExists, char *uname, char *hostname);
int config(int callKeygen);
void startup(char *username, char *hostname, char *password, int profileExists);
void sshLogin(char *uname, char *hostname);
void scpFrom(char *uname, char *hostname);
void scpTo(char *uname, char *hostname);