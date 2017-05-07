/**
 * @file binary_calculator.c
 *
 * @author Brendan Wilson
 *
 * @date 4/19/17
 *
 * Assignment: binary calculator
 *
 * @brief function material and logic for binary_calculator_main.c
 *
 * @details creates and manipulates a cpu struct with doubly linked registers
 *    to simulate the operations of that cpu.
 *
 * @bugs none
 */


#include "binary_calculator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#define LEN 1024


/**
 * @param *cpu is the cpu struct
 */
void valid_word(struct cpu_t *cpu)
{
        char buf[LEN];
        int valid_word = 0; //outside check
        while (valid_word != 1) {
                printf("Input the word length: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%d", &cpu -> word_size);
                if (cpu -> word_size < 1 || cpu -> word_size > 64) //outside 1-64
                        printf("Invalid Settings entered, word size must be between 1 and 64\n");
                else {
                        valid_word = 1; //exit while loop
                }
        }
        bin_expression(cpu);
}

/**
 * @param *cpu is the cpu struct
 */
void signed_unsigned(struct cpu_t *cpu)
{
        char buf[LEN];
        char *unsigned_signed = malloc(LEN);
        printf("Unsigned values [Y/n]: ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%c", unsigned_signed);
        if (unsigned_signed[0] == 'n')
                cpu -> unsign = 0;
        else
                cpu -> unsign = 1;
        free(unsigned_signed);
}

/**
 * @param *cpu is the cpu struct
 */
void bin_expression(struct cpu_t *cpu)
{
        char e[LEN];
        printf("Input the binary expression: ");
        fgets(e, LEN, stdin);
        char *expression_1 = malloc(LEN);
        expression_1 = strtok(e, " "); //first expression separation
        char *operator = malloc(LEN);
        operator = strtok(NULL, " "); //operator
        cpu -> operator = operator;
        char *expression_2 = malloc(LEN);
        expression_2 = strtok(NULL, "\n"); //second expression
        size_t len1 = strlen(expression_1); //length of first str
        size_t len2 = strlen(expression_2);
        char *exp_1 = malloc(len1 * sizeof(char));
        char *exp_2 = malloc(len2 * sizeof(char));
        unsigned char int_exp1[len1];
        unsigned char int_exp2[len2]; //%hhu for dereferencing

        int i;
        for (i = 0; i < len1; i++)
                *(exp_1 + i) = 0;
        for (i = 0; i < len2; i++)
                *(exp_2 + i) = 0;
        for (i = 0; i < len1; i++)
                sscanf((expression_1 + i), "%c", (exp_1 + i)); //convert to char array
        for (i = 0; i < len2; i++)
                sscanf((expression_2 + i), "%c", (exp_2 + i));
        for (i = 0; i < len1; i++)
                int_exp1[i] = *(exp_1 + i) - '0'; //convert to int array
        for (i = 0; i < len2; i++)
                int_exp2[i] = *(exp_2 + i) - '0';
        for (i = 0; i < len1; i++) { //check for invalid input
                if (int_exp1[i] != 0) {
                        if (int_exp1[i] != 1) {
                                printf("Invalid expression input - not 0 or 1. Retry\n");
                                main();
                        }
                }
                if (i + 1 > cpu -> word_size) {
                        printf("Invalid expression length. Retry\n");
                        main();
                }
        }
        for (i = 0; i < len2; i++) { //check for invalid input
                if (int_exp2[i] != 0) {
                        if (int_exp2[i] != 1) {
                                printf("Invalid expression input - not 0 or 1. Retry\n");
                                main();
                        }
                }
                if (i + 1 > cpu -> word_size) {
                        printf("Invalid expression length. Retry\n");
                        main();
                }
        }
        if (len1 < cpu -> word_size)
                pad_1(int_exp1, len1, cpu);
        if (len2 < cpu -> word_size)
                pad_2(int_exp2, len2, cpu);
        int size = cpu -> word_size;
        if (len1 == size)
                cpu -> r1_head = create_list(int_exp1, size, cpu -> r1_head, cpu);
        if (len2 == size)
                cpu -> r2_head = create_list(int_exp2, size, cpu -> r2_head, cpu);
}

/**
 * @param int_exp1 is the first expression int array
 * @param len1 is the size of the first int arrat
 * @param *cpu is the cpu struct
 */
void pad_1(unsigned char int_exp1[], int len1, struct cpu_t *cpu)
{
        int i;
        char c;
        int size = cpu -> word_size;
        unsigned char padded_1[size];
        int size1 = len1 - 1;
        for (i = 0; i < size1; i++) { //reverses the digit array, ex 100 becomes 001
        	c = int_exp1[i];
        	int_exp1[i] = int_exp1[size1];
        	int_exp1[size1] = c;
        	size1--;
        }
        for (i = 0; i < size; i++) //creates and prepares an array of cpu.word_size 0's
                padded_1[i] = 0;
        for (i = 0; i < len1; i++) //replaces first len1's digits with the digits from int_exp1
                padded_1[i] = int_exp1[i];
        size = size - 1;
        for (i = 0; i < size; i++) { //reverses the new array so that the 0's are first (or padded), 001000 becomes 000100 if ws is 6
        	c = padded_1[i];
        	padded_1[i] = padded_1[size];
        	padded_1[size] = c;
        	size--;
        }
        size = cpu -> word_size;
        cpu -> r1_head = create_list(padded_1, size, cpu -> r1_head, cpu);
}

/**
 * @param int_exp2 is the second expression int array
 * @param len1 is the size of the second int arrat
 * @param *cpu is the cpu struct
 */
void pad_2(unsigned char int_exp2[], int len2, struct cpu_t *cpu)
{
        int i;
        char c;
        int size = cpu -> word_size;
        int size2 = len2 - 1;
        unsigned char padded_2[size];
        for (i = 0; i < size2; i++) { //reverses the digit array, ex 100 becomes 001
        	c = int_exp2[i];
        	int_exp2[i] = int_exp2[size2];
        	int_exp2[size2] = c;
        	size2--;
        }
        for (i = 0; i < size; i++) //creates and prepares an array of cpu.word_size 0's
                padded_2[i] = 0;
        for (i = 0; i < len2; i++) //replaces first len2's digits with the digits from int_exp2
                padded_2[i] = int_exp2[i];
        size = size - 1;
        for (i = 0; i < size; i++) { //reverses the new array so that the 0's are first (or padded), 001000 becomes 000100 if ws is 6
        	c = padded_2[i];
        	padded_2[i] = padded_2[size];
        	padded_2[size] = c;
        	size--;
        }
        size = cpu -> word_size;
        cpu-> r2_head = create_list(padded_2, size, cpu -> r2_head, cpu);
}

/**
 * @param padded1 is the first expression int array, padded with 0's
 * @param size is the word size and size of thw arrays
 * @param *head is a generic param for the head of the passed in linked list
 * @param *cpu is the cpu struct
 * @return is the linked list
 */
struct bit_t *create_list(unsigned char padded_1[], int size, struct bit_t *head, struct cpu_t *cpu)
{
        int i;
        int a = 0;
        int a_init = padded_1[0];
        struct bit_t *tmp = NULL;
        struct bit_t *tmp_prev = NULL;
        struct bit_t *bit = NULL;

        head = insert_head(bit, a_init, head); //head creation
        tmp = head;
        tmp_prev = head;
        a = padded_1[1];
        bit = create_bit(a);
        tmp -> next = bit;
        tmp = tmp -> next;
        bit -> prev = tmp_prev;
        tmp_prev = tmp_prev -> next;

        for (i = 2; i < size; i++) {
                a = padded_1[i]; //sets the value at array[i] to a
                bit = create_bit(a);
                tmp -> next = bit; //links nodes
                tmp = tmp -> next; //increments tmp
                bit -> prev = tmp_prev; //sets the double link
                tmp_prev = tmp_prev -> next; //tmp and tmp_prev are on same node now
        }
        return head;
}

/**
 * @param *head is the generic passed in head value in the list
 * @param a_init is the initialization value to put in the list
 * @param *bit is the node
 * @return is the initialized bit/node
 */
struct bit_t *insert_head(struct bit_t *head, int a_init, struct bit_t *bit)
{
        bit = create_bit(a_init);
        bit -> next = head; //creates right hand connection to head
        head = bit; //head is now pointing to the new node
        return bit;
}

/**
 * @param a is the value to put into the bit/node
 * @return is the bit/node
 */
struct bit_t *create_bit(int a)
{
        struct bit_t *bit = malloc(sizeof(struct bit_t));
        bit -> n = a; //initializes the value system in the node
        bit -> next = NULL; //initializes the NULL terminator to the node
        bit -> prev = NULL;
        return bit;
}

/**
 * @param *cpu is the cpu struct
 * @param size is the wordsize/size of the arrays and linked lists
 */
void print_list(struct cpu_t *cpu, int size)
{
        int diff = cpu -> word_size - 1; //used in operator padding
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL;
        struct bit_t *tmp3 = NULL;
        tmp1 = cpu -> r1_head;
        tmp2 = cpu -> r2_head;
        tmp3 = cpu -> r3_head;
        int i;
        for(i = 0; i < size; i++) {
                printf("%hhu", tmp1 -> n); //first expression
                tmp1 = tmp1 -> next;
        }
        printf("\n");
        printf("%s", cpu -> operator);
        printf("\n");
        for (i = diff; i < size - 1; i++)
                printf(" "); //operator padding
        for (i = 0; i < size; i++) {
                printf("%hhu", tmp2 -> n); //second expression
                tmp2 = tmp2 -> next;
        }
        printf("\n");
        for (i = 0; i < size; i++)
                printf("-"); //dashed lines
        printf("\n");
        for (i = 0; i < size; i++) {
                printf("%hhu", tmp3 -> n); //answer expression
                tmp3 = tmp3 -> next;
        }
        printf("\n");
}

/**
 *@param *cpu is the cpu struct
 */
void add_func(struct cpu_t *cpu)
{
        struct bit_t *head1 = NULL;
        struct bit_t *head2 = NULL;
        struct bit_t *head3 = NULL;
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL;
        struct bit_t *tmp3 = NULL;
        struct bit_t *bit = NULL;
        struct bit_t *tmp3_prev = NULL;
        int carry = 0;
        int init_value = 0;

        int size = cpu -> word_size;
        head1 = cpu -> r1_head;
        head2 = cpu -> r2_head;
        head3 = insert_head(bit, init_value, head3); //creation of head node for r3
        tmp3 = head3;
        tmp3_prev = head3;
        bit = create_bit(init_value);
        tmp3 -> next = bit;
        tmp3 = tmp3 -> next;
        bit -> prev = tmp3_prev;
        tmp3_prev = tmp3_prev -> next;

        int i;
        for (i = 2; i < size; i++) {
                bit = create_bit(init_value); //initializes all values in r3_head to 0
                tmp3 -> next = bit; //links nodes
                tmp3 = tmp3 -> next; //increments tmp
                bit -> prev = tmp3_prev; //sets the double link
                tmp3_prev = tmp3_prev -> next; //tmp and tmp_prev are on same node now
        }
        cpu -> r3_head = head3;
        tmp1 = head1;
        tmp2 = head2;
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)  //next 3 loops set the tail of the list
                tmp1 = tmp1 -> next;

        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next;

        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next;
        //all tmp values are at the tail of the list currently, use tmp -> prev to iterate
        int tmp_val = 0;
        for (i = 0; i < size; i++) {
                tmp_val = ((tmp1 -> n) + (tmp2 -> n) + carry);
                if (tmp_val == 0)
                        carry = 0;
                else if (tmp_val == 1) {
                        tmp3 -> n = 1;
                        carry = 0;
                }
                else if (tmp_val == 2)
                        carry = 1;
                else if (tmp_val == 3) {
                        tmp3 -> n = 1;
                        carry = 1;
                }
                tmp1 = tmp1 -> prev;
                tmp2 = tmp2 -> prev;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        cpu -> r3_head = head3;
        if (carry == 1)
                cpu -> carry = 1;
        else
                cpu -> carry = 0;
}

/**
 * @param *cpu is the cpu struct
 */
void and_func(struct cpu_t *cpu)
{
        struct bit_t *head1 = NULL;
        struct bit_t *head2 = NULL;
        struct bit_t *head3 = NULL;
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL;
        struct bit_t *tmp3 = NULL;
        struct bit_t *bit = NULL;
        struct bit_t *tmp3_prev = NULL;
        int init_value = 0;

        int size = cpu -> word_size;
        head1 = cpu -> r1_head;
        head2 = cpu -> r2_head;
        head3 = insert_head(bit, init_value, head3); //creation of head node for r3
        tmp3 = head3;
        tmp3_prev = head3;
        bit = create_bit(init_value);
        tmp3 -> next = bit;
        tmp3 = tmp3 -> next;
        bit -> prev = tmp3_prev;
        tmp3_prev = tmp3_prev -> next;

        int i;
        for (i = 2; i < size; i++) {
                bit = create_bit(init_value); //initializes all values in r3_head to 0
                tmp3 -> next = bit; //links nodes
                tmp3 = tmp3 -> next; //increments tmp
                bit -> prev = tmp3_prev; //sets the double link
                tmp3_prev = tmp3_prev -> next; //tmp and tmp_prev are on same node now
        }
        cpu -> r3_head = head3;
        tmp1 = head1;
        tmp2 = head2;
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)  //next 3 loops set the tail of the list
                tmp1 = tmp1 -> next;

        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next;

        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next;
        //all tmp values are at the tail of the list currently, use tmp -> prev to iterate
        for (i = 0; i < size; i++) {
                if ((tmp1 -> n == 1) && (tmp2 -> n == 1))
                        tmp3 -> n = 1;
                tmp1 = tmp1 -> prev;
                tmp2 = tmp2 -> prev;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        cpu -> r3_head = head3;
        cpu -> carry = 0;
        cpu -> overflow = 0;
}

/**
 * @param *cpu is the cpu struct
 */
void or_func(struct cpu_t *cpu)
{
        struct bit_t *head1 = NULL;
        struct bit_t *head2 = NULL;
        struct bit_t *head3 = NULL;
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL;
        struct bit_t *tmp3 = NULL;
        struct bit_t *bit = NULL;
        struct bit_t *tmp3_prev = NULL;
        int init_value = 0;

        int size = cpu -> word_size;
        head1 = cpu -> r1_head;
        head2 = cpu -> r2_head;
        head3 = insert_head(bit, init_value, head3); //creation of head node for r3
        tmp3 = head3;
        tmp3_prev = head3;
        bit = create_bit(init_value);
        tmp3 -> next = bit;
        tmp3 = tmp3 -> next;
        bit -> prev = tmp3_prev;
        tmp3_prev = tmp3_prev -> next;

        int i;
        for (i = 2; i < size; i++) {
                bit = create_bit(init_value); //initializes all values in r3_head to 0
                tmp3 -> next = bit; //links nodes
                tmp3 = tmp3 -> next; //increments tmp
                bit -> prev = tmp3_prev; //sets the double link
                tmp3_prev = tmp3_prev -> next; //tmp and tmp_prev are on same node now
        }
        cpu -> r3_head = head3;
        tmp1 = head1;
        tmp2 = head2;
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)  //next 3 loops set the tail of the list
                tmp1 = tmp1 -> next;

        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next;

        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next;
        //all tmp values are at the tail of the list currently, use tmp -> prev to iterate
        for (i = 0; i < size; i++) {
                if ((tmp1 -> n == 1) || (tmp2 -> n == 1))
                        tmp3 -> n = 1;
                tmp1 = tmp1 -> prev;
                tmp2 = tmp2 -> prev;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        cpu -> r3_head = head3;
        cpu -> carry = 0;
        cpu -> overflow = 0;
}

/**
 * @param *cpu is the cpu struct
 */
void xor_func(struct cpu_t *cpu)
{
        struct bit_t *head1 = NULL;
        struct bit_t *head2 = NULL;
        struct bit_t *head3 = NULL;
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL;
        struct bit_t *tmp3 = NULL;
        struct bit_t *bit = NULL;
        struct bit_t *tmp3_prev = NULL;
        int init_value = 0;

        int size = cpu -> word_size;
        head1 = cpu -> r1_head;
        head2 = cpu -> r2_head;
        head3 = insert_head(bit, init_value, head3); //creation of head node for r3
        tmp3 = head3;
        tmp3_prev = head3;
        bit = create_bit(init_value);
        tmp3 -> next = bit;
        tmp3 = tmp3 -> next;
        bit -> prev = tmp3_prev;
        tmp3_prev = tmp3_prev -> next;

        int i;
        for (i = 2; i < size; i++) {
                bit = create_bit(init_value); //initializes all values in r3_head to 0
                tmp3 -> next = bit; //links nodes
                tmp3 = tmp3 -> next; //increments tmp
                bit -> prev = tmp3_prev; //sets the double link
                tmp3_prev = tmp3_prev -> next; //tmp and tmp_prev are on same node now
        }
        cpu -> r3_head = head3;
        tmp1 = head1;
        tmp2 = head2;
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)  //next 3 loops set the tail of the list
                tmp1 = tmp1 -> next;

        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next;

        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next;
        //all tmp values are at the tail of the list currently, use tmp -> prev to iterate
        for (i = 0; i < size; i++) {
                if ((tmp1 -> n == 0) && (tmp2 -> n == 1))
                        tmp3 -> n = 1;
                else if ((tmp1 -> n == 1) && (tmp2 -> n == 0))
                        tmp3 -> n = 1;
                tmp1 = tmp1 -> prev;
                tmp2 = tmp2 -> prev;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        cpu -> r3_head = head3;
        cpu -> carry = 0;
        cpu -> overflow = 0;
}

/**
 * @param *cpu is the cpu struct
 */
void flag_check(struct cpu_t *cpu)
{
        struct bit_t *tmp3 = NULL;
        struct bit_t *head3 = NULL;
        head3 = cpu -> r3_head;
        int size = cpu -> word_size;
        int count_1s = 0;
        tmp3 = head3;
        printf("Carry: %d\n", cpu -> carry);
        if (tmp3 -> n == 1) { //sign check
                printf("Sign: 1\n");
                cpu -> sign = 1;
        }
        else {
                printf("Sign: 0\n");
                cpu -> sign = 0;
        }
        int i;
        tmp3 = head3;
        for (i = 0; i < size; i++) {
                if (tmp3 -> n == 1)  //number of 1's count for parity flag/zero flag
                        count_1s = count_1s + 1;
                tmp3 = tmp3 -> next;
        }
        if (count_1s != 0) {
                printf("Zero: 0\n");
                cpu -> zero = 0;
        }
        else if (count_1s == 0) {
                printf("Zero: 1\n");
                cpu -> zero = 1;
        }
        if ((count_1s % 2 == 0) && (cpu -> zero == 0)) { //if even, parity is 1
                printf("Parity: 1\n");
                cpu -> parity = 1;
        }
        else {
                printf("Parity: 0\n");
                cpu -> parity = 0;
        }
}

/**
 * @param *cpu is the cpu struct
 */
void decimal_convert(struct cpu_t *cpu)
{
        struct bit_t *tmp3 = NULL;
        int pow_val = 0;
        int size = cpu -> word_size;
        int i;
        int count = 0;
        tmp3 = cpu -> r3_head;
        int add_vals[size]; //will store all pow values to add together to form a decimal number
        for (i = 0; i < size; i++)
                add_vals[i] = 0;
        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next; //tmp3 on tail of list

        for (i = 0; i < size; i++) {
                if (tmp3 -> n == 1) {
                        pow_val = pow(2, i);
                        add_vals[i] = pow_val; //appends the pow value to add_vals[i]
                }
                tmp3 = tmp3 -> prev; //incrementer
        }
        for (i = 0; i < size; i++)
                count = add_vals[i] + count; //gets the total count
	if (cpu -> carry == 1)
		count = count + pow(2, size + 1);
        printf("Decimal equivalent: %d\n", count);
}

/**
 * @param *cpu is the cpu struct
 */
void signed_convert(struct cpu_t *cpu)
{
        struct bit_t *tmp3 = NULL;
        struct bit_t *head3 = NULL;
        int size = cpu -> word_size;
        int i;
        tmp3 = cpu -> r3_head;
        head3 = cpu -> r3_head;
        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next; //tmp3 on tail of list
        for (i = 0; i < size; i++) {
                if (tmp3 -> n == 0)
                        tmp3 -> n = 1;
                else if (tmp3 -> n == 1)
                        tmp3 -> n = 0;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next; //tmp3 on tail of list
        int carry = 0;
        tmp3 -> n = tmp3 -> n + 1; //adds 1 to the lsb
        for (i = 0; i < size; i++) {
                tmp3 -> n = tmp3 -> n + carry; //adds carry to tmp3 -> n
                if (tmp3 -> n == 2) {
                        tmp3 -> n = 0; //carry will be 1, tmp3 -> n is 0
                        carry = 1;
                }
                else
                        carry = 0; //once carry is 0, no other elems in tmp3 will be affected value wise
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        neg_decimal_convert(cpu, tmp3);
}


/**
 * @param *cpu is the cpu struct
 * @param neg_val is the modified linked list to use
 */
void neg_decimal_convert(struct cpu_t *cpu, struct bit_t *neg_val)
{
        struct bit_t *tmp = NULL;
        int pow_val = 0;
        int size = cpu -> word_size;
        int i;
        int count = 0;
        tmp = neg_val;
        int add_vals[size]; //will store all pow values to add together to form a decimal number
        for (i = 0; i < size; i++)
                add_vals[i] = 0;
        for (i = 0; i < size - 1; i++)
                tmp = tmp -> next; //tmp3 on tail of list

        for (i = 0; i < size; i++) {
                if (tmp -> n == 1) {
                        pow_val = pow(2, i);
                        add_vals[i] = pow_val; //appends the pow value to add_vals[i]
                }
                tmp = tmp -> prev; //incrementer
        }
        for (i = 0; i < size; i++)
                count = add_vals[i] + count; //gets the total count
        printf("Negative decimal equivalent: -%d\n", count);
}

/**
 * @param *cpu is the cpu struct
 */
void subtract_func(struct cpu_t *cpu)
{
        struct bit_t *tmp1 = NULL;
        struct bit_t *tmp2 = NULL; //similar process in signed_convert but on r2_head
        struct bit_t *tmp3 = NULL;
        struct bit_t *head1 = NULL;
        struct bit_t *head2 = NULL;
        struct bit_t *head3 = NULL;
        struct bit_t *bit = NULL;
        struct bit_t *tmp3_prev = NULL;
        struct bit_t *replace_bit2 = NULL;
        int size = cpu -> word_size;
        int i;

        tmp2 = cpu -> r2_head;
        head2 = cpu -> r2_head;
        replace_bit2 = cpu -> r2_head;
        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next; //tmp3 on tail of list
        for (i = 0; i < size; i++) {
                if (tmp2 -> n == 0)
                        tmp2 -> n = 1;
                else if (tmp2 -> n == 1)
                        tmp2 -> n = 0;
                tmp2 = tmp2 -> prev;
        }
        tmp2 = head2;
        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next; //tmp3 on tail of list
        int carry = 0;
        tmp2 -> n = tmp2 -> n + 1;
        for (i = 0; i < size; i++) {
                tmp2 -> n = tmp2 -> n + carry;
                if (tmp2 -> n == 2) {
                        tmp2 -> n = 0;
                        carry = 1;
                }
                else
                        carry = 0;
                tmp2 = tmp2 -> prev;
        }

        int init_value = 0;

        head1 = cpu -> r1_head;
        head2 = cpu -> r2_head;
        head3 = insert_head(bit, init_value, head3); //creation of head node for r3
        tmp3 = head3;
        tmp3_prev = head3;
        bit = create_bit(init_value);
        tmp3 -> next = bit;
        tmp3 = tmp3 -> next;
        bit -> prev = tmp3_prev;
        tmp3_prev = tmp3_prev -> next;

        for (i = 2; i < size; i++) {
                bit = create_bit(init_value); //initializes all values in r3_head to 0
                tmp3 -> next = bit; //links nodes
                tmp3 = tmp3 -> next; //increments tmp
                bit -> prev = tmp3_prev; //sets the double link
                tmp3_prev = tmp3_prev -> next; //tmp and tmp_prev are on same node now
        }
        cpu -> r3_head = head3;
        tmp1 = head1;
        tmp2 = head2;
        tmp3 = head3;
        for (i = 0; i < size - 1; i++)  //next 3 loops set the tail of the list
                tmp1 = tmp1 -> next;

        for (i = 0; i < size - 1; i++)
                tmp2 = tmp2 -> next;

        for (i = 0; i < size - 1; i++)
                tmp3 = tmp3 -> next;
        //all tmp values are at the tail of the list currently, use tmp -> prev to iterate
        int tmp_val = 0;
        for (i = 0; i < size; i++) {
                tmp_val = ((tmp1 -> n) + (tmp2 -> n) + carry);
                if (tmp_val == 0)
                        carry = 0;
                else if (tmp_val == 1) {
                        tmp3 -> n = 1;
                        carry = 0;
                }
                else if (tmp_val == 2)
                        carry = 1;
                else if (tmp_val == 3) {
                        tmp3 -> n = 1;
                        carry = 1;
                }
                tmp1 = tmp1 -> prev;
                tmp2 = tmp2 -> prev;
                tmp3 = tmp3 -> prev;
        }
        tmp3 = head3;
        cpu -> r2_head = replace_bit2;
}


/**
 * @param1 is the generic head pointer
 */
void delete_list(struct bit_t *head)
{
        struct bit_t *tmp_next = NULL;
        tmp_next = head -> next;
        free(head); //solves a logic hole in while loop that disincludes deleting the first node
        if (head != NULL) { //0 list check
                while (tmp_next != NULL) { //traverses list until null
                        head = tmp_next;
                        tmp_next = tmp_next -> next; //staggered pointers
                        free(head);
                }
        }
}
