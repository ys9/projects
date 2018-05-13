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
 *
 */

/*
  _______          _____ _____ _    _ 
 |__   __|        / ____/ ____| |  | |
    | |_   ___  _| (___| (___ | |__| |
    | | | | \ \/ /\___ \\___ \|  __  |
    | | |_| |>  < ____) |___) | |  | |
    |_|\__,_/_/\_\_____/_____/|_|  |_|

*/


#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>
#include <termios.h>
#include "ssh.h"
#define BLUE "\x1b[34m"
#define GREEN "\x1b[32m"
#define RESET "\x1b[0m"
#define LEN 1024


int main(int argc, char *argv[])
{
	system("clear");
	char buf[LEN];
	char *uname = calloc(30, sizeof(char));
	assert(uname);
	char *hostname = calloc(50, sizeof(char));
	assert(hostname);
	char *password = calloc(100, sizeof(char));
	assert(password);
	printf(BLUE "    _______          _____ _____ _    _ \n");
	printf("   |__   __|        / ____/ ____| |  | |\n");
	printf("      | |_   ___  _| (___| (___ | |__| |\n");
	printf("      | | | | \\ \\/ /\\___  \\___ \\|  __  |\n");
	printf("      | | |_| |>  < ____) |___) | |  | |\n");
	printf("      |_|\\__,_/_/\\_\\_____/_____/|_|  |_|\n\n\n" RESET);
	printf("Username: ");
	fgets(buf, LEN, stdin);
	sscanf(buf, "%s", uname);
	printf("Hostname: ");
	fgets(buf, LEN, stdin);
	sscanf(buf, "%s", hostname);
	printf("Password: ");
    struct termios term, term_orig; //password masking 
    tcgetattr(STDIN_FILENO, &term);
    term_orig = term;
    term.c_lflag &= ~ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fgets(buf, LEN, stdin);
    strncpy(password, buf, strlen(buf));
    tcsetattr(STDIN_FILENO, TCSANOW, &term_orig);
    system("clear");
    for (int i = 0; i < strlen(password); i++) {
		if (password[i] == '\n')
				password[i] = '\0';
	}
	int profileExists = 0;
	profileExists = checkProfiles(profileExists, uname, hostname);
	startup(uname, hostname, password, profileExists);

	int menuChoice = 0;
	while (1) {
		printf(BLUE "    _______          _____ _____ _    _ \n");
		printf("   |__   __|        / ____/ ____| |  | |\n");
		printf("      | |_   ___  _| (___| (___ | |__| |\n");
		printf("      | | | | \\ \\/ /\\___  \\___ \\|  __  |\n");
		printf("      | | |_| |>  < ____) |___) | |  | |\n");
		printf("      |_|\\__,_/_/\\_\\_____/_____/|_|  |_|\n\n\n" RESET);
		printf(GREEN "(1) SSH into machine\n(2) Upload files TO server\n(3) Download files FROM Server\n(4) About / License\n(5) Quit\n>>> " RESET);
		fgets(buf, LEN, stdin);
		sscanf(buf, "%d", &menuChoice);
		if (menuChoice == 1) {
			sshLogin(uname, hostname);
			system("clear");
		}
		else if (menuChoice == 2) {
			scpTo(uname, hostname);
		}
		else if (menuChoice == 3) {
			scpFrom(uname, hostname);
		}
		else if (menuChoice == 4) {
			about();
		}
		else if (menuChoice == 5) {
			system("clear");
			free(uname);
			free(hostname);
			free(password);
			exit(EXIT_SUCCESS);
		}
		else {
			system("clear");
			printf("Input not understood, retrying\n\n");
			//Reenter loop
		}
	}
}


/**
 * @param profileExists is a 0 or 1 that indicates whether the uname and host
 * have been used in the program before, used for determining to call
 * keygen / ssh-copy-id later or not
 * @param uname is the username on the server
 * @param hostname is the hostname of the server
 * @param password is the password of the user on the server
 */
int checkProfiles(int profileExists, char *uname, char *hostname)
{
	FILE *fp;
	char buf[2048];
	char *inputLine = calloc(80, sizeof(char));
	assert(inputLine);
	char *testUname = calloc(30, sizeof(char));
	assert(testUname);
	char *testHostname = calloc(50, sizeof(char));
	assert(testHostname);
	fp = fopen(".profiles", "r");
	while(fgets(buf, LEN, fp)) {
    	sscanf(buf, "%s", inputLine); //copy whole line in file to inputLine
    	testUname = strtok(inputLine, ","); //delimit hostname separated by ,
    	testHostname = strtok(NULL, "\0"); //uname is the rest of the line
    	if ((strcmp(testUname, uname) == 0) && strcmp(testHostname, hostname) == 0) //if uname and hostname match what is in file
    		profileExists = 1;
    	else {
    		inputLine = calloc(80, sizeof(char)); //clear memory for reuse in loop
    		testUname = calloc(30, sizeof(char));
    		testHostname = calloc(50, sizeof(char));
    	}
    }
    fclose(fp);
    if (profileExists == 0) {
    	char *cmd = malloc(120);
    	assert(cmd);
    	cmd[0] = '\0';
    	char *pre = "echo ";
    	char *post = " | tee -a .profiles > /dev/null";
    	cmd = strcat(cmd, pre);
    	cmd = strcat(cmd, uname);
    	cmd = strcat(cmd, ",");
    	cmd = strcat(cmd, hostname);
    	cmd = strcat(cmd, post);
    	system(cmd);
    	free(cmd);
    }
    return profileExists;
}


/**
 * @param uname is the username on the server
 * @param hostname is the hostname of the server
 * @param password is the password of the user on the server
 */
void startup(char *uname, char *hostname, char *password, int profileExists)
{
	//call scripts to generate ssh keys and add to remote server
	//but first, call config file to see if this program has been ran before
	int callKeygen = 0;
	callKeygen = config(callKeygen);
	if (callKeygen == 0) {
		printf("Creating keys...\n");
		system("./.scripts/.ssh-keygen > /dev/null"); //create ssh keys, no output to user
	}
	if (profileExists == 0) {
		char *path = "./.scripts/.copyKey ";
		char *null = " > /dev/null";
		char *space = " ";
		char *cmd = malloc(200);
		assert(cmd);
		cmd[0] = '\0';
		cmd = strcat(cmd, path);
		cmd = strncat(cmd, uname, 30);
		cmd = strcat(cmd, space);
		cmd = strncat(cmd, hostname, 50);
		cmd = strcat(cmd, space);
		cmd = strncat(cmd, password, 100);
		cmd = strcat(cmd, null);
		printf("Copying key to destination...\n");
		system(cmd);
		free(cmd);
	}
	system("clear");
}


/**
 * @param callKeygen is a 0 or 1 indicating if the program has been ran before or not. If 
 * it has, then ssh-keygen will not be called. If it has, ssh-keygen will be called.
 */
int config(int callKeygen)
{
	FILE *fp;
	char buf[10];
	fp = fopen(".config", "r");
	while(fgets(buf, LEN, fp)) {
    	sscanf(buf, "%d", &callKeygen);
    }
    if (callKeygen == 0)
    	system("echo 1 | tee .config > /dev/null"); //replaces 0 with 1 for next run
    fclose(fp);
    return callKeygen;
}


/**
 * @param uname is the username on the server
 * @param hostname is the hostname of the server
 * @param password is the password of the user on the server
 */
void sshLogin(char *uname, char *hostname)
{
	printf("Executing...\n");
    char *cmd = calloc(220, sizeof(char));
    assert(cmd);
    cmd[0] = '\0';
    char *path = "./.scripts/.sshLogin ";
    char *userSpace = "-u ";
    char *hostSpace = " -h ";
    cmd = strcat(cmd, path);
    cmd = strcat(cmd, userSpace);
    cmd = strncat(cmd, uname, 30);
    cmd = strcat(cmd, hostSpace);
    cmd = strncat(cmd, hostname, 50); //will resemble ./.sshLogin -u uname -h hostname
    system(cmd); //calls ssh auto-login script
    free(cmd);
}


/**
 * @param uname is the username on the server
 * @param hostname is the hostname of the server
 * @param password is the password of the user on the server
 */
void scpFrom(char *uname, char *hostname)
{
	int fileNum = 0;
	char *inputLine = calloc(50, sizeof(char)); //str for the current line
	assert(inputLine);
	inputLine[0] = '\0';
	char **fileArr; //array of strings that holds files to be uploaded
	char *buf = malloc(LEN * 2);
	assert(buf);
	buf[0] = '\0';
	printf("Download: Enter files / directories one per line, enter 'done' when done entering files\n");
	while (1) {
		inputLine = calloc(50, sizeof(char)); //clears line to null terminators
		inputLine[0] = '\0';
		fileNum++;
		fgets(inputLine, LEN, stdin);
		if (strcmp(inputLine, "done\n") == 0)
			break;
		buf = strncat(buf, inputLine, 50);
	}
	fileNum--; //removes 'done' from file count
	fileArr = malloc(fileNum * sizeof(char *));
	for (int i = 0; i < fileNum; i++) {
		fileArr[i] = malloc(50);
	}
	int fileCounter = 0;
	int arrayCounter = 0;
	inputLine = calloc(50, sizeof(char));
	inputLine[0] = '\0';
	for (int i = 0; i < strlen(buf); i++) { //goes through buf char by char and copies information to fileArr
		if (buf[i] == '\n') { //skip over newline, don't put it in fileArr
			fileArr[fileCounter] = inputLine;
			fileCounter++;
			arrayCounter = 0;
			inputLine = calloc(50, sizeof(char));
			inputLine[0] = '\0';
		}
		else { //char
			inputLine[arrayCounter] = buf[i];
			arrayCounter++;
		}
	}
	free(buf);
	free(inputLine);
	char *path = "./.scripts/.scpFrom -u ";
	char *space = " ";
	char *hostSpace = " -h ";
	char *cmd = malloc(1000);
	assert(cmd);
	cmd[0] = '\0';
	assert(cmd);
	cmd = strcat(cmd, path);
	cmd = strncat(cmd, uname, 30);
	cmd = strcat(cmd, hostSpace);
	cmd = strncat(cmd, hostname, 50);
	for (int i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == '\n')
			cmd[i] = '\0';
	}
	for (int i = 0; i < fileNum; i++) {
		cmd = strcat(cmd, space);
		cmd = strcat(cmd, fileArr[i]);
	}
	system("clear");
	printf("%s\n", cmd);
	system(cmd);
	free(cmd);
	for (int i = 0; i < fileNum; i++)
		free(fileArr[i]);
	free(fileArr);
	printf("\n");
}


/**
 * @param uname is the username on the server
 * @param hostname is the hostname of the server
 * @param password is the password of the user on the server
 */
void scpTo(char *uname, char *hostname)
{
	int fileNum = 0;
	char *inputLine = calloc(50, sizeof(char)); //str for the current line
	assert(inputLine);
	inputLine[0] = '\0';
	char **fileArr; //array of strings that holds files to be uploaded
	char *buf = malloc(LEN * 2);
	assert(buf);
	buf[0] = '\0';
	printf("Upload: Enter files / directories one per line, enter 'done' when done entering files\n");
	while (1) {
		inputLine = calloc(50, sizeof(char)); //clears line to null terminators
		inputLine[0] = '\0';
		fileNum++;
		fgets(inputLine, LEN, stdin);
		if (strcmp(inputLine, "done\n") == 0)
			break;
		buf = strncat(buf, inputLine, 50);
	}
	fileNum--; //removes 'done' from file count
	fileArr = malloc(fileNum * sizeof(char *));
	for (int i = 0; i < fileNum; i++) 
		fileArr[i] = malloc(50);
	int fileCounter = 0;
	int arrayCounter = 0;
	inputLine = calloc(50, sizeof(char));
	inputLine[0] = '\0';
	for (int i = 0; i < strlen(buf); i++) { //goes through buf char by char and copies information to fileArr
		if (buf[i] == '\n') { //skip over newline, don't put it in fileArr
			fileArr[fileCounter] = inputLine;
			fileCounter++;
			arrayCounter = 0;
			inputLine = calloc(50, sizeof(char));
			inputLine[0] = '\0';
		}
		else { //char
			inputLine[arrayCounter] = buf[i];
			arrayCounter++;
		}
	}
	free(buf);
	free(inputLine);
	char *path = "./.scripts/.scpTo -u ";
	char *space = " ";
	char *hostSpace = " -h ";
	char *cmd = malloc(1000);
	assert(cmd);
	cmd[0] = '\0';
	assert(cmd);
	cmd = strcat(cmd, path);
	cmd = strncat(cmd, uname, 30);
	cmd = strcat(cmd, hostSpace);
	cmd = strncat(cmd, hostname, 50);
	for (int i = 0; i < strlen(cmd); i++) {
		if (cmd[i] == '\n')
			cmd[i] = '\0';
	}
	for (int i = 0; i < fileNum; i++) {
		cmd = strcat(cmd, space);
		cmd = strcat(cmd, fileArr[i]);
	}
	system("clear");
	printf("%s\n", cmd);
	system(cmd);
	free(cmd);
	for (int i = 0; i < fileNum; i++)
		free(fileArr[i]);
	free(fileArr);
	printf("\n");
}


void about(void)
{
	char buf[100];
	char exitCond;
	while (1) {
		system("clear");
		printf(BLUE "    _______          _____ _____ _    _ \n");
		printf("   |__   __|        / ____/ ____| |  | |\n");
		printf("      | |_   ___  _| (___| (___ | |__| |\n");
		printf("      | | | | \\ \\/ /\\___  \\___ \\|  __  |\n");
		printf("      | | |_| |>  < ____) |___) | |  | |\n");
		printf("      |_|\\__,_/_/\\_\\_____/_____/|_|  |_|\n\n\n" RESET);
		printf("Developed and Created by Brendan Wilson (c) 2018\n");
		printf("Managed as a FOSS program, maintained at https://github.com/bmw417/projects/tree/master/TuxSSH\n");
		printf("Licensing and License Information at https://github.com/bmw417/projects/blob/master/TuxSSH/LICENSE.md\n\n");
		printf("Contact me at brendan.wilson@student.nmt.edu for questions or comments\n\n");
		printf("Enter anything to quit to menu: ");
		fgets(buf, 100, stdin);
		sscanf(buf, "%c", &exitCond);
		if (exitCond != '\0')
			system("clear");
			break;
	}
}
