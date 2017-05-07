/**
 * @file binary_calculator_main.c
 *
 * @author Brendan Wilson
 *
 * @date 4/19/17
 *
 * Assignment: binary calculator
 *
 * @brief function material and logic for lbinary_calculator__main.c
 *
 * @details creates and manipulates a cpu struct with doubly linked registers
 *    to simulate the operations of that cpu.
 *
 * @bugs none
 */

#include "binary_calculator.h"
#define LEN 1024
#include <stdio.h>
#include <stdlib.h>

 int main(void)
 {
         LOOP: while(1) {
                 struct cpu_t *cpu = NULL;
                 struct bit_t *bit = NULL;
                 char buf[LEN];
                 char *choice_continue = malloc(LEN);
                 cpu = malloc(sizeof(struct cpu_t));
                 bit = malloc(sizeof(struct bit_t));
                 valid_word(cpu);
                 signed_unsigned(cpu);
                 int size = cpu -> word_size;
                 switch(cpu -> operator[0]) { //index char * for reading
                 case '+':
                         add_func(cpu);
                         if (cpu -> unsign == 0) {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 signed_convert(cpu);
                         }
                         else {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 decimal_convert(cpu);
                         }
                         printf("Continue [y/N]: ");
                         fgets(buf, LEN, stdin);
                         sscanf(buf, "%c", choice_continue);
                         if (choice_continue[0] == 'y') {
                                 goto LOOP;
                                 printf("\n");
                         }
                         else {
                                 printf("Goodbye\n");
                                 delete_list(cpu -> r1_head);
                                 delete_list(cpu -> r2_head);
                                 delete_list(cpu -> r3_head);
                                 free(cpu -> r1_head);
                                 free(cpu -> r2_head);
                                 free(cpu -> r3_head);
                                 free(cpu);
                                 free(bit);
                                 free(choice_continue);
                                 exit(1);
                         }
                         break;
                 case '&':
                         and_func(cpu);
                         if (cpu -> unsign == 0) {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 signed_convert(cpu);
                         }
                         else {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 decimal_convert(cpu);
                         }
                         printf("Continue [y/N]: ");
                         fgets(buf, LEN, stdin);
                         sscanf(buf, "%c", choice_continue);
                         if (choice_continue[0] == 'y') {
                                 goto LOOP;
                                 printf("\n");
                         }
                         else {
                                 printf("Goodbye\n");
                                 delete_list(cpu -> r1_head);
                                 delete_list(cpu -> r2_head);
                                 delete_list(cpu -> r3_head);
                                 free(cpu -> r1_head);
                                 free(cpu -> r2_head);
                                 free(cpu -> r3_head);
                                 free(cpu);
                                 free(bit);
                                 free(choice_continue);
                                 exit(1);
                         }
                         break;
                 case '|':
                         or_func(cpu);
                         if (cpu -> unsign == 0) {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 signed_convert(cpu);
                         }
                         else {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 decimal_convert(cpu);
                         }
                         printf("Continue [y/N]: ");
                         fgets(buf, LEN, stdin);
                         sscanf(buf, "%c", choice_continue);
                         if (choice_continue[0] == 'y') {
                                 goto LOOP;
                                 printf("\n");
                         }
                         else {
                                 printf("Goodbye\n");
                                 delete_list(cpu -> r1_head);
                                 delete_list(cpu -> r2_head);
                                 delete_list(cpu -> r3_head);
                                 free(cpu -> r1_head);
                                 free(cpu -> r2_head);
                                 free(cpu -> r3_head);
                                 free(cpu);
                                 free(bit);
                                 free(choice_continue);
                                 exit(1);
                         }
                         break;
                 case '^':
                         xor_func(cpu);
                         if (cpu -> unsign == 0) {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 signed_convert(cpu);
                         }
                         else {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 decimal_convert(cpu);
                         }
                         printf("Continue [y/N]: ");
                         fgets(buf, LEN, stdin);
                         sscanf(buf, "%c", choice_continue);
                         if (choice_continue[0] == 'y') {
                                 goto LOOP;
                                 printf("\n");
                         }
                         else {
                                 printf("Goodbye\n");
                                 delete_list(cpu -> r1_head);
                                 delete_list(cpu -> r2_head);
                                 delete_list(cpu -> r3_head);
                                 free(cpu -> r1_head);
                                 free(cpu -> r2_head);
                                 free(cpu -> r3_head);
                                 free(cpu);
                                 free(bit);
                                 free(choice_continue);
                                 exit(1);
                         }
                         break;
                 case '-':
                         subtract_func(cpu);
                         if (cpu -> unsign == 0) {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 signed_convert(cpu);
                         }
                         else {
                                 print_list(cpu, size);
                                 flag_check(cpu);
                                 decimal_convert(cpu);
                         }
                         printf("Continue [y/N]: ");
                         fgets(buf, LEN, stdin);
                         sscanf(buf, "%c", choice_continue);
                         if (choice_continue[0] == 'y') {
                                 goto LOOP;
                                 printf("\n");
                         }
                         else {
                                 printf("Goodbye\n");
                                 delete_list(cpu -> r1_head);
                                 delete_list(cpu -> r2_head);
                                 delete_list(cpu -> r3_head);
                                 free(cpu -> r1_head);
                                 free(cpu -> r2_head);
                                 free(cpu -> r3_head);
                                 free(cpu -> operator);
                                 free(cpu);
                                 free(bit);
                                 free(choice_continue);
                                 exit(1);
                         }
                         break;
                 default:
                         printf("Invalid input for operator\n");
                         goto LOOP;
                         break;
                 }
         }
 }
