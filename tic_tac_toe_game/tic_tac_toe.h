#ifndef TIC_TAC_TOE_H_
#define TIC_TAC_TOE_H_

struct game_position_t {
        int player1[9];
        int player2[9];
        int total_moves[9];
};

unsigned char **init_matrix(int rows, int cols);
unsigned char **zero_funct(unsigned char **a, int rows, int cols);
unsigned char **sdl_board_setup(unsigned char **a, int rows, int cols);
void init_struct(struct game_position_t *game_positions);
unsigned char **player1_move(unsigned char **a, int rows, int cols, struct game_position_t *game_positions);
unsigned char **player1_draw(unsigned char **a, struct game_position_t *game_positions, int player1_move, int rows, int cols);
unsigned char **player2_move(unsigned char **a, int rows, int cols, struct game_position_t *game_positions);
unsigned char **player2_draw(unsigned char **a, struct game_position_t *game_positions, int player2_move, int rows, int cols);
int player1_win_check(struct game_position_t *game_positions, int win);
int player2_win_check(struct game_position_t *game_positions, int win);
int check_cat_game(struct game_position_t *game_positions, int cat);

#endif
