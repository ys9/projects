/**
 * @author: Brendan Wilson
 *
 * @date 4/12/17
 *
 * Assignment: Lab 8
 *
 * @brief a program that creates and manipulates linked lists and nodes
 *
 * @details creates, deletes, and inserts nodes at various points to make a
 *    singly linked list
 *
 * @bugs none
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
enum {INSERT_HEAD = 1, INSERT_MIDDLE, INSERT_TAIL, PRINT_LIST, DELETE_NODE, COUNT_NODES, FIND_NODE, QUIT};
#define MENU_LEN 32
#define LEN 1024

struct node_t {
        double x;
        struct node_t *next;
};

struct node_t *create_node(double n);
void print_node(struct node_t *node);
void print_list(struct node_t *head);
struct node_t *insert_head(struct node_t *head, struct node_t *node);
struct node_t *insert_tail(struct node_t *head, struct node_t *node);
struct node_t *insert_middle(struct node_t *head, struct node_t *node, int pos);
int count_nodes(struct node_t *head);
struct node_t *find_node(struct node_t *head, double n);
struct node_t *delete_node(struct node_t *head, double n);
void delete_list(struct node_t *head);

int main(void) {
        char buf[MENU_LEN];
	int menu;
        int count, pos = 0;
        double n = 0.0;
        struct node_t *node = NULL;
        struct node_t *head = NULL;
        struct node_t *found_node = NULL;

	while (1) {
                printf("Welcome to Node-Mart! Enter a Selection:\n");
                printf("1. Insert head node\n");
                printf("2. Insert middle node\n");
                printf("3. Insert tail node\n");
                printf("4. Print the list of nodes\n");
                printf("5. Delete a node\n");
                printf("6. Count all nodes in the list\n");
                printf("7. Find a node if it exists in the list\n");
                printf("8. Quit program\n");
		printf("Enter selection: ");
		fgets(buf, LEN, stdin);
		menu = atoi(buf);


	switch(menu) {
        case INSERT_HEAD:
                head = insert_head(head, node);
                printf("\n");
                break;
        case INSERT_MIDDLE:
                head = insert_middle(head, node, pos);
                printf("\n");
                break;
        case INSERT_TAIL:
                head = insert_tail(head, node);
                printf("\n");
                break;
        case PRINT_LIST:
                print_list(head);
                break;
        case DELETE_NODE:
                head = delete_node(head, n);
                printf("\n");
                break;
        case COUNT_NODES:
                count = count_nodes(head);
                printf("There are %d linked elements total\n\n", count);
                break;
        case FIND_NODE:
                found_node = find_node(head, n);
                if (found_node == NULL)
                        printf("The number was not found in the list\n\n");
                else
                        print_node(found_node);
                break;
	case QUIT:
	        printf("Goodbye\n");
		if (head == NULL)
			exit(EXIT_SUCCESS);
		else {
                	delete_list(head);
	        	exit(EXIT_SUCCESS);
		}
		break;
	default:
		printf("Invalid input\n\n");
		break;
	}
	}

return 0;
}

/**
 * @param1 is the number to attach to the link list
 * @return is the node struct
 */
struct node_t *create_node(double n)
{
        char buf[LEN];
        printf("Please enter in the node value: ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%lf", &n);
        struct node_t *node = malloc(sizeof(struct node_t));
        node -> x = n; //initializes the value system in the node
        node -> next = NULL; //initializes the NULL terminator to the node
        return node;
}

/**
 * @param1 is the head pointer struct
 * @param2 is the node pointer struct
 * @return is the head pointer
 */
struct node_t *insert_head(struct node_t *head, struct node_t *node)
{
        double m = 0.0;
        node = create_node(m);
        node -> next = head; //creates right hand connection to head
        head = node; //head is now pointing to the new node
        return head;
}

/**
 * @param1 is the head pointer struct
 * @param2 is the node pointer struct
 * @param3 is the position that the node will be put in
 * @return is the head pointer
 */
struct node_t *insert_middle(struct node_t *head, struct node_t *node, int pos)
{
        double m = 0.0;
        node = create_node(m);
        struct node_t *tmp = NULL;
        struct node_t *after = NULL;
        char buf[LEN];
        printf("Note: inserts by index, not count\n");
        printf("Enter the position to insert the node: ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%d", &pos);
        int count = 0;
        tmp = head;
        while (tmp != NULL) { //counts all elems in link list
                tmp = tmp -> next;
                count += 1;
        }
        if (pos > count)
                pos = count; //seg fault won't happen because if user enters a pos beyond the
        tmp = head;          //link list, it will set pos equal to input.
        if (count == 0) { //check for empty list
                head = node;
                node -> next = NULL;
        }
        else {
                int i = 0;
                for (; i < pos; i++) {
                        if (i == pos - 1) { //pos -1 because links are made to the node after our created node
                                after = tmp -> next; //staggered pointers, tmp and after
                                tmp -> next = node; //makes left side connection
                                node -> next = after; //makes right side connection
                                break; //won't execute tmp -> next if in this if statement
                        }
                        else
                                tmp = tmp -> next;
                }
        }
        return head;
}

/**
 * @param1 is the head pointer struct
 * @param2 is the node struct
 * @return is the head pointer
 */
struct node_t *insert_tail(struct node_t *head, struct node_t *node)
{
        int count = 0;
        int m = 0.0;
        node = create_node(m);
        struct node_t *tmp = NULL;
        tmp = head;
        while (tmp != NULL) {
                tmp = tmp -> next;
                count += 1;
        }
        int i = 0;
        tmp = head;
        if (count == 0) { //check for empty list
                head = node;
                node -> next = NULL;
        }
        else {
                for (; i < count; i++) {
                        if (i == count - 1) {
                                tmp -> next = node;
                                //node -> next is already NULL, so only one line is needed
                                break;
                        }
                        else
                                tmp = tmp -> next; //incrementer
                }
        }
        return head;
}

/**
 * @param1 is the head pointer struct
 * @return is the count of all nodes
 */
int count_nodes(struct node_t *head)
{
        int count = 0;
        struct node_t *tmp = NULL;
        tmp = head;
        while (tmp != NULL) {
                count += 1;
                tmp = tmp -> next;
        }
        return count;
}

/**
 * @param1 is the head pointer struct
 */
void print_list(struct node_t *head)
{
        int count = 0;
        struct node_t *tmp = NULL;
        tmp = head;
        while (tmp != NULL) {
                tmp = tmp -> next;
                count += 1;
        } //count of all elems in link list
        tmp = head;
        int i = 0, inc = 0; //incrementer in for loop
        for(; i < count; i++) {
                printf("Node %d - value: %lf, address: %p\n", inc, tmp -> x, tmp -> next);
                tmp = tmp -> next;
                inc += 1;
        }
        printf("\n");
}

/**
 * @param1 is the head pointer struct
 * @param2 is the node value to look for to delete
 * @return is the head pointer
 */
struct node_t *delete_node(struct node_t *head, double n)
{
        int count = 0;
        int i = 0;
        int pos = -1;
        struct node_t *tmp = NULL;
        struct node_t *tmp_next = NULL;
        struct node_t *tmp_next_next = NULL;
        char buf[LEN];
        printf("Enter the value to delete: ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%lf", &n);
        tmp = head;
        while (tmp != NULL) {
                count += 1;
                tmp = tmp -> next;
        }
        tmp = head;
        for(; i < count; i++) {
                if (isgreaterequal(n, tmp -> x) && islessequal(n, tmp -> x)) {
                        pos = i; //check for if n = node(x) anywhere, and keeps the pos if true
                        break;
                }
                tmp = tmp -> next;
        }
        tmp = head; //reset tmp
        i = 0; //reset i
        if (pos == -1) //value not found
                printf("Element with value %lf not found\n", n);
        else if (pos == 0) { //if pos is head pointer
                tmp = tmp -> next; //tmp is at index of 1
                free(head);
                head = tmp;
        }
        else if (pos == count - 1) { //tail of list, -1 for index / count disparity
                for (; i < count; i++) { //iterates until pos is reached
                        if (i == pos - 1) {
                                tmp_next = tmp -> next; //tmp_next on tail, tmp on tail - 1
                                free(tmp_next); //tail freed
                                tmp -> next = NULL; //new tail set to NULL
                                break;
                        }
                        tmp = tmp -> next; //increment tmp if pos != i
                }
        }
        else {
                for (; i < pos; i++) {
                        if (i == pos - 1) { //pos - 1 to 'sandwich' the node to delete, tmp is on node before the one to delete
                                tmp_next = tmp -> next; //tmp_next is now the node to delete
                                tmp_next_next = tmp -> next -> next; //tmp_next_next is the node after the one to delete
                                tmp -> next = tmp_next_next;
                                free(tmp_next);
                        }
                        tmp = tmp -> next;
                }
        }
        return head;
}

/**
 * @param1 is the head pointer
 */
void delete_list(struct node_t *head)
{
        struct node_t *tmp_next = NULL;
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

/**
 * @param1 is the head struct
 * @param2 is the value of n to try and find in the link lists
 * @param return is either NULL or a pointer to tmp
 */
struct node_t *find_node(struct node_t *head, double n)
{
        struct node_t *tmp = NULL;
        char buf[LEN];
        printf("Enter the value to search for: ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%lf", &n);
        tmp = head;
        while (tmp != NULL) {
                if (tmp -> x == n)
                        return tmp;
                tmp = tmp -> next;
        }
        return NULL;
}

/**
 * @param1 is the node struct
 */
void print_node(struct node_t *node)
{
        struct node_t *tmp = NULL;
        tmp = node -> next;
        printf("The node with value %lf was found, it's address \n"
               "is %p and the next node's address is %p\n\n", node -> x, node, tmp);
}
