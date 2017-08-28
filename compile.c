#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define LEN 1024


void one_part_compile(void);
void three_part_compile(void);


int main(void)
{
        char *main_option = malloc(LEN);
        char buf[LEN];
        LOOP: while(1) {
                printf("Input (1) for single program compile, (2) for 3 part compile, or (3) for exit: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%c", main_option);
                if (main_option[0] == '1')
                        one_part_compile();
                else if (main_option[0] == '2')
                        three_part_compile();
                else if (main_option[0] == '3')
                        exit(1);
                else {
                        printf("Invalid input, retrying\n");
                        goto LOOP;
                }
        }
        free(main_option);
}

void one_part_compile(void)
{
        char *left_command = "gcc -g -Wall ";
        char *right_command = " -o ";
        char *combined1;
        char *link_math = " -lm";
        char *math_option = malloc(LEN);
        char buf[LEN];

        char *program_name = malloc(LEN);
        printf("Input program name without file extension: ");
        fgets(program_name, LEN, stdin);
        program_name = strtok(program_name, "\n");
        char *c = ".c";
        char *c_program = malloc(LEN);
        size_t c_len = strlen(program_name);
        strncat(c_program, program_name, c_len);
        strncat(c_program, c, c_len);

        size_t len = strlen(left_command) + strlen(right_command) + strlen(program_name) + strlen(c_program) + strlen(link_math) + 3;
        combined1 = malloc(len * sizeof(char));
        strncat(combined1, left_command, len);
        strncat(combined1, c_program, len);
        strncat(combined1, right_command, len);
        strncat(combined1, program_name, len);

        printf("Math linking needed? (y/n): ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%c", math_option);
        LOOP: while(1) {
                if (math_option[0] == 'y') {
                        strncat(combined1, link_math, len);
                        printf("%s\n", combined1);
                        break;
                }
                else if (math_option[0] == 'n') {
                        printf("%s\n", combined1);
                        break;
                }
                else {
                        printf("Invalid input, retrying\n");
                        goto LOOP;
                }
        }

        char *run = "./";
        char *combined2;
        size_t run_len = strlen(program_name) + 5;
        combined2 = malloc(run_len * sizeof(char));
        strncat(combined2, run, run_len);
        strncat(combined2, program_name, run_len);

        system(combined1);
        system(combined2);

        free(program_name);
        free(c_program);
        free(math_option);
        free(combined1);
        free(combined2);
}


void three_part_compile(void)
{
        char *left_command = "gcc -g -Wall ";
        char *dash_o = " -o ";
        char *dash_c = "-c ";
        char *run = "./";
        char *link_math = " -lm";
        char *math_option = malloc(LEN);
        char *combined1;
        char *combined2;
        char buf[LEN];

        char *logic = malloc(LEN);
        char *main_file = malloc(LEN);
        printf("Input logic file with no extension: ");
        fgets(logic, LEN, stdin);
        printf("Input main file with no extension: ");
        fgets(main_file, LEN, stdin);
        main_file = strtok(main_file, "\n");
        logic = strtok(logic, "\n");
        char *c = ".c";
        char *o = ".o ";
        char *logic_c = malloc(LEN);
        char *logic_o = malloc(LEN);
        char *main_c = malloc(LEN);
        size_t c_logic_len = strlen(logic) + strlen(c);
        size_t c_main_len = strlen(main_file) + strlen(c);
        strncat(logic_c, logic, c_logic_len);
        strncat(logic_c, c, c_logic_len);
        strncat(logic_o, logic, c_logic_len);
        strncat(logic_o, o, c_logic_len);
        strncat(main_c, main_file, c_main_len);
        strncat(main_c, c, c_main_len);

        size_t len1 = strlen(left_command) + strlen(dash_c) + strlen(logic_c) + 2;
        size_t len2 = strlen(left_command) + strlen(logic_o) + strlen(main_c) + strlen(dash_o) + strlen(main_file) + strlen(link_math) + 5;
        combined1 = malloc(len1 * sizeof(char));
        combined2 = malloc(len2 * sizeof(char));

        strncat(combined1, left_command, len1);
        strncat(combined1, dash_c, len1);
        strncat(combined1, logic_c, len1);
        strncat(combined2, left_command, len2);
        strncat(combined2, logic_o, len2);
        strncat(combined2, main_c, len2);
        strncat(combined2, dash_o, len2);
        strncat(combined2, main_file, len2);

        printf("Math linking needed? (y/n): ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%c", math_option);
        LOOP: while(1) {
                if (math_option[0] == 'y') {
                        strncat(combined2, link_math, len2);
                        break;
                }
                else if (math_option[0] == 'n') {
                        break;
                }
                else {
                        printf("Invalid input, retrying\n");
                        goto LOOP;
                }
        }

        size_t run_len = strlen(run) + strlen(main_file);
        char *combined_run = malloc(run_len * sizeof(char));
        strncat(combined_run, run, run_len);
        strncat(combined_run, main_file, run_len);

        printf("%s\n", combined1);
        printf("%s\n", combined2);
        printf("%s\n", combined_run);
        system(combined1);
        system(combined2);
        system(combined_run);

        free(logic);
        free(main_file);
        free(logic_o);
        free(logic_c);
        free(main_c);
        free(combined1);
        free(combined2);
        free(math_option);
}
