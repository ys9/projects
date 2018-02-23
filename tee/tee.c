/**
 * @file tee.c
 *
 * @author Brendan Wilson
 *
 * @date 2/22/17
 *
 * @brief runs a copy of tee using sys calls and file manipulation.
 *
 * @details uses apue.h error handing protocols. Checks for piped input from
 * stdin, checks getopts for append/overwrite mode, and then either appends
 * or overwrites file with input from stdin.
 *
 * @bugs none
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include "apue.h"
#define EXIT_SUCCESS 0
#define STDIN_FAIL_ERR 1
#define INVALID_PARAMS 2
#define OPEN_FILE_FAIL 3
#define LEN 1024

void usage(void);

int main(int argc, char *argv[]) {
        int appendFlag = 0;
        int i = 0;
        char buf[LEN];
        if (!isatty(fileno(stdin))) {
                while (-1 != (buf[i++] = getchar()));
                buf[i - 1] = '\0';
                for (i = 0; i < strlen(buf); i++) {
                        if (buf[i] == '\n')
                                buf[i] = '\0';
                } //reads piped/redirected input, remove newline
        }
        else {
                err_sys("ERROR: No input from stdin detected, use  echo [input] or cat [file]\n");
                exit(STDIN_FAIL_ERR); //no input from stdin
        }
        int opt = 0;
        char *file = malloc(25);
        assert(file); //check for malloc failure
        while ((opt = getopt(argc, argv, "ha:")) != -1) {
                switch (opt) {
                        case 'h':
                                usage();
                                exit(EXIT_SUCCESS);
                                break;
                        case 'a':
                                appendFlag = 1;
                                file = optarg;
                                break;
                        default:
                                usage();
                                err_sys("ERROR: Invalid parameter usage\n");
                                exit(INVALID_PARAMS);
                                break;
                }
        }
        if (argc == 2) {
                file = argv[1];
        }
        if (appendFlag == 0) { //open in write (overwrites all content) mode
                FILE *fp1;
                fp1 = fopen(file, "w");
                if (fp1 == NULL) {
                        err_sys("ERROR: File open failed\n");
                        exit(OPEN_FILE_FAIL);
                }
                fprintf(fp1, "%s\n", buf); //writes piped buf to file
                fclose(fp1);
        }
        else { //open in append (preserve other file contents) mode
                FILE *fp2;
                fp2 = fopen(file, "a");
                if (fp2 == NULL) {
                        err_sys("ERROR: File open failed\n");
                        exit(OPEN_FILE_FAIL);
                }
                fprintf(fp2, "%s\n", buf);
                fclose(fp2);
        }
}

/**
 *usage function for getopt -h option and misuse of getopt
 */
void usage(void)
{
        printf("echo [input] | ./tee [-OPTION] file\n");
        printf("cat [file] | ./tee [-OPTION] file\n");
        printf("OPTION: -a: appends to end of file\n");
        printf("Default (no option): overwrites file with input from stdin\n");
}
