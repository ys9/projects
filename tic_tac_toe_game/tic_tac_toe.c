#include "tic_tac_toe.h"
#include <stdio.h>
#include <stdlib.h>
#define LEN 512


/**
 * @param rows: vertical array
 * @param cols: horizontal array
 * @return: returns the initialized array of arrays
 */
unsigned char **init_matrix(int rows, int cols)
{
        int i;
        int j;
        unsigned char **a;
        /* allocate rows */
        a = malloc(rows * sizeof(int *));
        if(!a)
                return NULL;
        for(i = 0; i < rows; i++) {
                /* allocate cols for each row */
                a[i] = malloc(cols * sizeof(int));
                if (!a[i]) {
                        for (j = 0; j < i; j++)
                                free(a[j]);
                free(a);
                return NULL;
                }
        }
        return a;
}

/**
 * @param a is the matrix
 * @param rows is the vertical array
 * @param cols is the horizontal array
 */
unsigned char **zero_funct(unsigned char **a, int rows, int cols)
{
        int i, j;
        for (i = 0; i < rows; i++) {
                for (j = 0; j < cols; j++) {
                        a[i][j] = 0;
                }
        }
        return a;
}

/**
 * @param a is the matrix
 * @param rows are the rows of the matrix
 * @param cols are the columns of the matrix
 * @returns the matrix
 */
unsigned char **sdl_board_setup(unsigned char **a, int rows, int cols)
{
        int rows_division = rows / 3;
        int cols_division = cols / 3;
        int i;
        for (i = 5; i < cols - 5; i++)
                a[rows_division][i] = 1;
        for (i = 5; i < cols - 5; i++)
                a[2 * rows_division][i] = 1;
        for (i = 10; i < rows - 10; i++)
                a[i][cols_division] = 1;
        for (i = 10; i < rows - 10; i++)
                a[i][2 * cols_division] = 1;
        return a;
}

/**
 * @param game_positions is the struct that holds the three arrays that keep track of player turns
 */
void init_struct(struct game_position_t *game_positions)
{
        int i;
        for (i = 0; i < 9; i++)
                game_positions -> player1[i] = 0;
        for (i = 0; i < 9; i++)
                game_positions -> player2[i] = 0;
        for (i = 0; i < 9; i++)
                game_positions -> total_moves[i] = 0;
}

/**
 * @param a is the matrix
 * @param rows are the rows of the matrix
 * @param cols are the cols of the matrix
 * @param game_positions is the struct containing player moves
 * @returns the matrix
 */
unsigned char **player1_move(unsigned char **a, int rows, int cols, struct game_position_t *game_positions)
{
        int player1_move;
        char buf[LEN];
        LOOP: while (1) {
                printf("--PLAYER 1--\n");
                printf("1 2 3\n");
                printf("4 5 6\n");
                printf("7 8 9\n");
                printf("Choose a move: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%d", &player1_move);
                if (player1_move == 10)
                        exit(EXIT_SUCCESS);
                else if (player1_move < 0 || player1_move > 9) {
                        printf("Invalid move entered, retrying\n");
                        goto LOOP;
                }
                else if (game_positions -> total_moves[player1_move - 1] == 1) {
                        printf("Move already made, try another\n");
                        goto LOOP;
                }
                break;
        }
        printf("\n");
        game_positions -> player1[player1_move - 1] = 1;
        game_positions -> total_moves[player1_move - 1] = 1;
        a = player1_draw(a, game_positions, player1_move, rows, cols);
        return a;
}

/**
 * @param a is the matrix
 * @param game_positions is the struct that holds the player moves
 * @param player1_move is the cell number player 1 entered
 * @param rows are the rows of the matrix
 * @param cols are the cols of the matrix
 * @returns the matrix
 */
unsigned char **player1_draw(unsigned char **a, struct game_position_t *game_positions, int player1_move, int rows, int cols)
{
        int i;
        if (player1_move == 1) {
                a[20][14] = 1;
                for (i = 1; i < 5; i++) {
                        a[20 + i][14 + i] = 1;
                        a[20 - i][14 + i] = 1;
                        a[20 + i][14 - i] = 1;
                        a[20 - i][14 - i] = 1;
                }
        }
        else if (player1_move == 2) {
                a[50][14] = 1;
                for (i = 1; i < 5; i++) {
                        a[50 + i][14 + i] = 1;
                        a[50 - i][14 + i] = 1;
                        a[50 + i][14 - i] = 1;
                        a[50 - i][14 - i] = 1;
                }
        }
        else if (player1_move == 3) {
                a[79][14] = 1;
                for (i = 1; i < 5; i++) {
                        a[79 + i][14 + i] = 1;
                        a[79 - i][14 + i] = 1;
                        a[79 + i][14 - i] = 1;
                        a[79 - i][14 - i] = 1;
                }
        }
        else if (player1_move == 4) {
                a[20][38] = 1;
                for (i = 1; i < 5; i++) {
                        a[20 + i][38 + i] = 1;
                        a[20 - i][38 + i] = 1;
                        a[20 + i][38 - i] = 1;
                        a[20 - i][38 - i] = 1;
                }
        }
        else if (player1_move == 5) {
                a[50][38] = 1;
                for (i = 1; i < 5; i++) {
                        a[50 + i][38 + i] = 1;
                        a[50 - i][38 + i] = 1;
                        a[50 + i][38 - i] = 1;
                        a[50 - i][38 - i] = 1;
                }
        }
        else if (player1_move == 6) {
                a[79][38] = 1;
                for (i = 1; i < 5; i++) {
                        a[79 + i][38 + i] = 1;
                        a[79 - i][38 + i] = 1;
                        a[79 + i][38 - i] = 1;
                        a[79 - i][38 - i] = 1;
                }
        }
        else if (player1_move == 7) {
                a[20][61] = 1;
                for (i = 1; i < 5; i++) {
                        a[20 + i][61 + i] = 1;
                        a[20 - i][61 + i] = 1;
                        a[20 + i][61 - i] = 1;
                        a[20 - i][61 - i] = 1;
                }
        }
        else if (player1_move == 8) {
                a[50][61] = 1;
                for (i = 1; i < 5; i++) {
                        a[50 + i][61 + i] = 1;
                        a[50 - i][61 + i] = 1;
                        a[50 + i][61 - i] = 1;
                        a[50 - i][61 - i] = 1;
                }
        }
        else if (player1_move == 9) {
                a[79][61] = 1;
                for (i = 1; i < 5; i++) {
                        a[79 + i][61 + i] = 1;
                        a[79 - i][61 + i] = 1;
                        a[79 + i][61 - i] = 1;
                        a[79 - i][61 - i] = 1;
                }
        }
        return a;
}

/**
 * @param a is the matrix
 * @param rows are the rows of the matrix
 * @param cols are the cols of the matrix
 * @param game_positions is the struct containing player moves
 * @returns the matrix
 */
unsigned char **player2_move(unsigned char **a, int rows, int cols, struct game_position_t *game_positions)
{
        int player2_move;
        char buf[LEN];
        LOOP: while (1) {
                printf("--PLAYER 2--\n");
                printf("1 2 3\n");
                printf("4 5 6\n");
                printf("7 8 9\n");
                printf("Choose a move: ");
                fgets(buf, LEN, stdin);
                sscanf(buf, "%d", &player2_move);
                if (player2_move == 10)
                        exit(EXIT_SUCCESS);
                else if (player2_move < 0 || player2_move > 9) {
                        printf("Invalid move entered, retrying\n");
                        goto LOOP;
                }
                else if (game_positions -> total_moves[player2_move - 1] == 1) {
                        printf("Move already made, try another\n");
                        goto LOOP;
                }
                break;
        }
        printf("\n");
        game_positions -> player2[player2_move - 1] = 1;
        game_positions -> total_moves[player2_move - 1] = 1;
        a = player2_draw(a, game_positions, player2_move, rows, cols);
        return a;
}

/**
 * @param a is the matrix
 * @param game_positions is the struct that holds the player moves
 * @param player2_move is the cell number player 2 entered
 * @param rows are the rows of the matrix
 * @param cols are the cols of the matrix
 * @returns the matrix
 */
unsigned char **player2_draw(unsigned char **a, struct game_position_t *game_positions, int player2_move, int rows, int cols)
{
        int i;
        if (player2_move == 1) {
                a[14][9] = 1;
                for (i = 1; i < 10; i++) {
                        a[14 + i][9] = 1;
                        a[14][9 + i] = 1;
                        a[14 + i][18] = 1;
                        a[23][9 + i] = 1;
                }
        }
        else if (player2_move == 2) {
                a[44][9] = 1;
                for (i = 1; i < 10; i++) {
                        a[44 + i][9] = 1;
                        a[44][9 + i] = 1;
                        a[44 + i][18] = 1;
                        a[53][9 + i] = 1;
                }
        }
        else if (player2_move == 3) {
                a[73][9] = 1;
                for (i = 1; i < 10; i++) {
                        a[73 + i][9] = 1;
                        a[73][9 + i] = 1;
                        a[73 + i][18] = 1;
                        a[82][9 + i] = 1;
                }
        }
        else if (player2_move == 4) {
                a[14][33] = 1;
                for (i = 1; i < 10; i++) {
                        a[14 + i][33] = 1;
                        a[14][33 + i] = 1;
                        a[14 + i][42] = 1;
                        a[23][33 + i] = 1;
                }
        }
        else if (player2_move == 5) {
                a[44][33] = 1;
                for (i = 1; i < 10; i++) {
                        a[44 + i][33] = 1;
                        a[44][33 + i] = 1;
                        a[44 + i][42] = 1;
                        a[53][33 + i] = 1;
                }
        }
        else if (player2_move == 6) {
                a[73][33] = 1;
                for (i = 1; i < 10; i++) {
                        a[73 + i][33] = 1;
                        a[73][33 + i] = 1;
                        a[73 + i][42] = 1;
                        a[82][33 + i] = 1;
                }
        }
        else if (player2_move == 7) {
                a[14][56] = 1;
                for (i = 1; i < 10; i++) {
                        a[14 + i][56] = 1;
                        a[14][56 + i] = 1;
                        a[14 + i][65] = 1;
                        a[23][56 + i] = 1;
                }
        }
        else if (player2_move == 8) {
                a[44][56] = 1;
                for (i = 1; i < 10; i++) {
                        a[44 + i][56] = 1;
                        a[44][56 + i] = 1;
                        a[44 + i][65] = 1;
                        a[53][56 + i] = 1;
                }
        }
        else if (player2_move == 9) {
                a[73][56] = 1;
                for (i = 1; i < 10; i++) {
                        a[73 + i][56] = 1;
                        a[73][56 + i] = 1;
                        a[73 + i][65] = 1;
                        a[82][56 + i] = 1;
                }
        }
        return a;
}

/**
 * @param game_positions is the struct holding player moves
 * @param win is a 0 (no win) or 1 (win)
 * @returns win, a 0 or 1
 */
int player1_win_check(struct game_position_t *game_positions, int win)
{
        if ((game_positions -> player1[0] == 1) && (game_positions -> player1[1] == 1) && (game_positions -> player1[2] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[3] == 1) && (game_positions -> player1[4] == 1) && (game_positions -> player1[5] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[6] == 1) && (game_positions -> player1[7] == 1) && (game_positions -> player1[8] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[0] == 1) && (game_positions -> player1[4] == 1) && (game_positions -> player1[8] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[6] == 1) && (game_positions -> player1[4] == 1) && (game_positions -> player1[2] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[0] == 1) && (game_positions -> player1[3] == 1) && (game_positions -> player1[6] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[1] == 1) && (game_positions -> player1[4] == 1) && (game_positions -> player1[7] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player1[2] == 1) && (game_positions -> player1[5] == 1) && (game_positions -> player1[8] == 1)) {
                printf("Player 1 WINS!\n");
                win = 1;
                return win;
        }
        return win;
}


/**
 * @param game_positions is the struct holding player moves
 * @param win is a 0 (no win) or 1 (win)
 * @returns win, a 0 or 1
 */
int player2_win_check(struct game_position_t *game_positions, int win)
{
        if ((game_positions -> player2[0] == 1) && (game_positions -> player2[1] == 1) && (game_positions -> player2[2] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[3] == 1) && (game_positions -> player2[4] == 1) && (game_positions -> player2[5] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[6] == 1) && (game_positions -> player2[7] == 1) && (game_positions -> player2[8] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[0] == 1) && (game_positions -> player2[4] == 1) && (game_positions -> player2[8] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[6] == 1) && (game_positions -> player2[4] == 1) && (game_positions -> player2[2] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[0] == 1) && (game_positions -> player2[3] == 1) && (game_positions -> player2[6] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[1] == 1) && (game_positions -> player2[4] == 1) && (game_positions -> player2[7] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        else if ((game_positions -> player2[2] == 1) && (game_positions -> player2[5] == 1) && (game_positions -> player2[8] == 1)) {
                printf("Player 2 WINS!\n");
                win = 1;
                return win;
        }
        return win;
}

/**
 * @param game_positions is the struct that holds player moves
 * @param cat is a 0 (no tie game) or 1 (tie game)
 * @param returns cat (0 or 1)
 */
int check_cat_game(struct game_position_t *game_positions, int cat)
{
        int i;
        for (i = 0; i < 8; i++) {
                if (game_positions -> total_moves[i] == 0)
                        return cat;
        }
        cat = 1;
        return cat;
}
