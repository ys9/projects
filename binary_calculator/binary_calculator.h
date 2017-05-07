/**
 * @file binary_calculator.h
 *
 * @author Brendan Wilson
 *
 * @date 4/19/17
 *
 * Assignment: binary calculator
 *
 * @brief function prototypes for binary_calculator_main.c
 *
 * @details creates and manipulates a cpu struct with doubly linked registers
 *    to simulate the operations of that cpu.
 *
 * @bugs none
 */


 #include <stdlib.h>
 #define LEN 1024

 struct cpu_t {
         int word_size;
         int unsign; //0 -- signed, 1 for unsigned
         char *operator;
         //flags
         int overflow;
         int carry;
         int sign;
         int parity;
         int zero;
         struct bit_t *r1_head;
         struct bit_t *r1_tail;
         struct bit_t *r2_head;
         struct bit_t *r2_tail;
         struct bit_t *r3_head;
         struct bit_t *r3_tail;
};


struct bit_t {
         unsigned char n; /* store either 0 or 1 not ’0’ or ’1’ */
         struct bit_t *prev;
         struct bit_t *next;
 };

int main(void);
void valid_word(struct cpu_t *cpu);
void signed_unsigned(struct cpu_t *cpu);
void bin_expression(struct cpu_t *cpu);
void pad_1(unsigned char int_exp1[], int len1, struct cpu_t *cpu);
void pad_2(unsigned char int_exp2[], int len2, struct cpu_t *cpu);
struct bit_t *create_list(unsigned char padded_1[], int size, struct bit_t *head, struct cpu_t *cpu);
struct bit_t *insert_head(struct bit_t *head, int a_init, struct bit_t *bit);
struct bit_t *create_bit(int n);
void print_list(struct cpu_t *cpu, int size);
void add_func(struct cpu_t *cpu);
void and_func(struct cpu_t *cpu);
void or_func(struct cpu_t *cpu);
void xor_func(struct cpu_t *cpu);
void flag_check(struct cpu_t *cpu);
void decimal_convert(struct cpu_t *cpu);
void signed_convert(struct cpu_t *cpu);
void neg_decimal_convert(struct cpu_t *cpu, struct bit_t *neg_val);
void subtract_func(struct cpu_t *cpu);
void delete_list(struct bit_t *head);
