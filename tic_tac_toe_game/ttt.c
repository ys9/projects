#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h"
#include "sdl.h"
#include "tic_tac_toe.h"
#define LEN 512

int main()
{
        int red = 125;
        int green = 125;
        int blue = 125;
        int width = 800;
        int height = 600;
        int sprite_size = 8;
        struct game_position_t *game_positions = NULL;
        game_positions = malloc(sizeof(struct game_position_t));
        unsigned char **a;
        struct sdl_info_t sdl_info;
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

        int rows = (width / sprite_size);
        int cols = (height / sprite_size);
        a = init_matrix(rows, cols);
        a = zero_funct(a, rows, cols);
        a = sdl_board_setup(a, rows, cols);
        sdl_render_life(&sdl_info, a);
        init_struct(game_positions);
        int count = 1;
        int win = 0;
        int cat = 0;
        printf("Enter '10' at any time to terminate program\n");

	LOOP: while (1) {
		if (SDL_GetTicks() % 150 == 0) {
                        if (count % 2 == 1) {
                                a = player1_move(a, rows, cols, game_positions);
                                sdl_render_life(&sdl_info, a);
                                win = player1_win_check(game_positions, win);
                                cat = check_cat_game(game_positions, cat);
                                if (win == 1)
                                        break;
                                if (cat == 1) {
                                        printf("Tie Game!\n");
                                        break;
                                }
                        }
                        if (count % 2 == 0) {
                                a = player2_move(a, rows, cols, game_positions);
                                sdl_render_life(&sdl_info, a);
                                win = player2_win_check(game_positions, win);
                                if (win == 1)
                                        break;
                        }
                        count++;
                }
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			switch (event.type) {
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return 0;
				break;
			case SDL_QUIT:
				return(0);
			}
		}
        }
        char buf[LEN];
        char option;
        printf("Play Again [Y/n]? ");
        fgets(buf, LEN, stdin);
        sscanf(buf, "%c", &option);
        printf("\n");
        if (option == 'n') {
                free(game_positions);
                exit(EXIT_SUCCESS);
        }
        else {
                a = zero_funct(a, rows, cols);
                a = sdl_board_setup(a, rows, cols);
                init_struct(game_positions);
                win = 0;
                cat = 0;
                sdl_render_life(&sdl_info, a);
                goto LOOP;
        }
	return 0;
}
