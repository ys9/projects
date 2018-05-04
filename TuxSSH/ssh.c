/**
 * @file ssh.c
 *
 * @author Brendan Wilson
 *
 * @date 4/29/2018
 *
 * @brief runs an ssh/scp terminal client in C and bash. Logging into servers, 
 * copying files from a server, and copying files to a server are all supported.
 * Copying directories and multiple files is also supported.
 */
//cmd to create directory over ssh: ssh user@host "mkdir -p dir -m [umask] (get permission bits from statbuf)"


#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include <ftw.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include "ssh.h"
#define LEN 1024


int main(int argc, char *argv[])
{
	char buf[LEN];
	int menuChoice = 0;
	while (1) {
		system("clear");
		printf("(1) SSH into machine\n(2) SCP files FROM server\n(3) SCP files TO Server\n(4) Quit\n>>> ");
		fgets(buf, LEN, stdin);
		sscanf(buf, "%d", &menuChoice);
		if (menuChoice == 1) {
			sshLogin();
		}
		else if (menuChoice == 2) {
			scpFrom();
		}
		else if (menuChoice == 3) {
			scpTo();
		}
		else if (menuChoice == 4) {
			system("clear");
			exit(EXIT_SUCCESS);
		}
		else {
			//Reenter loop
		}
	}
}


void sshLogin(void)
{
	char buf[LEN];
	char *uname = calloc(30, sizeof(char)); //set memory alloc to '\0's
	assert(uname);
	char *hostname = calloc(50, sizeof(char));
	assert(hostname);
	char *password = calloc(100, sizeof(char));
	assert(password);
	printf("Username: ");
	fgets(buf, LEN, stdin);
	sscanf(buf, "%s", uname);
	printf("Hostname: ");
	fgets(buf, LEN, stdin);
	sscanf(buf, "%s", hostname);
	printf("Password: ");
    struct termios term, term_orig; //lines 53-60 termios.h password masking protocol
    tcgetattr(STDIN_FILENO, &term);
    term_orig = term;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fgets(buf, LEN, stdin);
    strncpy(password, buf, strlen(buf));
    tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);

    char *cmd = calloc(220, sizeof(char));
    char *path = "./.sshLogin ";
    char *space = " ";
    cmd = strcat(cmd, path);
    cmd = strncat(cmd, uname, 30);
    cmd = strcat(cmd, space);
    cmd = strncat(cmd, hostname, 50);
    cmd = strcat(cmd, space);
    cmd = strncat(cmd, password, 100); //will resemble ./.sshLogin -u uname -h hostname -p password
    system(cmd);
    free(uname);
    free(hostname);
    free(password);
    free(cmd);
}


void scpFrom(void)
{
	int fileNum = 0;
	char *inputLine = calloc(50, sizeof(char)); //str for the current line
	assert(inputLine);
	char **fileArr;
	assert(fileArr);
	char *buf = malloc(LEN * 2);
	assert(buf);
	printf("Enter files / directories one per line, enter 'done' when done entering files\n");
	while (1) {
		inputLine = calloc(50, sizeof(char)); //clears line to null terminators
		fileNum++;
		fgets(inputLine, LEN, stdin);
		if (strcmp(inputLine, "done\n") == 0)
			break;
		buf = strncat(buf, inputLine, 50);
	}
	fileNum--;
	fileArr = malloc(50 * fileNum);
	int fileCounter = 0;
	int arrayCounter = 0;
	inputLine = calloc(50, sizeof(char));
	for (int i = 0; i < strlen(buf); i++) {
		if (buf[i] == '\n') {
			printf("%s\n", inputLine);
			fileArr[fileCounter] = inputLine;
			fileCounter++;
			arrayCounter = 0;
			inputLine = calloc(50, sizeof(char));
		}
		else {
			inputLine[arrayCounter] = buf[i];
			arrayCounter++;
		}
	}
	free(buf);
	free(inputLine);
	struct stat *statbuf = calloc(sizeof(struct stat), sizeof(char));
	assert(statbuf);
	for (int i = 0; i < fileNum; i++) {
		statbuf = fillStruct(statbuf, fileArr[i]);
		if ((statbuf -> st_mode & S_IFMT) == S_IFDIR) {
			int permInt = statbuf -> st_mode;
        	permInt = dec2oct(permInt); //used later in permission section
        	char perms[6];
        	char *perm = malloc(3);
        	assert(perm);
        	sprintf(perms, "%d", permInt);
        	int counter = 0;
        	for (int i = strlen(perms) - 4; i < strlen(perms); i++) {
                perm[counter] = perms[i];
                counter++;
       		} //4 digit permission bits (ie 0777, 0644 etc)

		}
	}
}


void scpTo(void)
{

}


/**
 * @param statbuf is the struct that holds stat information on file
 * @param file is the path of the file or directory
 * @return returns the statbuf struct after being filled
 */
struct stat *fillStruct(struct stat *statbuf, char *file)
{
        if (lstat(file, statbuf) == -1) {
                char msg[256];
                sprintf(msg, "error in %s:%s:%d", __FILE__, "fillStruct", __LINE__);
                perror(msg);
        }
        return statbuf;
}


/**
 * @param num is the decimal number passed in from struct
 * @returns the decimal number converted to octal
 */
int dec2oct(int num)
{
        int oct = 0, i = 1;
        while (num != 0) {
                oct += (num % 8) * i;
                num /= 8;
                i *= 10;
        }
        return oct;
}