// Copyright 2018 Samoilescu Sebastian
#include <stdio.h>

#ifndef _X_0_FUNCTIONS_H
#define _X_0_FUNCTIONS_H

#define BOARD_MAX 100
#define CORN_NUMBER 2
#define PLAYERS 2
#define COORD_DIMENSION 2
#define INVALID -1

void initialize_board(int n, char board[][BOARD_MAX]);
void initialize_macro_board(int n, char macro_board[][BOARD_MAX]);
void print_macro_board(int n, char macro_board[][BOARD_MAX]);
void calculate_coord_of_boards(
    int n, int coord[BOARD_MAX][CORN_NUMBER][COORD_DIMENSION]);
char change_turn(char turn);
int is_not_empty(int x, int y, char board[][BOARD_MAX]);
int get_mini_board(int x, int y, int n);
int verify_line(int line,
                int coord_of_current_board[CORN_NUMBER][COORD_DIMENSION],
                char board[][BOARD_MAX], char player);
int verify_column(int column,
                  int coord_of_current_board[CORN_NUMBER][COORD_DIMENSION],
                  char board[][BOARD_MAX], char player);
int verify_main_diagonal(int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                         char board[][BOARD_MAX], char player);
int verify_second_diagonal(
    int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
    char board[][BOARD_MAX], char player);
int verify_mini_board(int line, int column,
                      int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                      char board[][BOARD_MAX], char player);
int update_macro_board(int line, int column, char board[][BOARD_MAX],
                       char player, int current_mboard_coord[][COORD_DIMENSION],
                       char macro_board[][BOARD_MAX], int n);
void update_coeffs(int generated, int coeffs[][PLAYERS], char player,
                   int type);
int get_random_position(char board[][BOARD_MAX], int n,
                        int generated_position[]);
void chose_winner(int line, int column, char player,
                  char macro_board[][BOARD_MAX], int n, char *winner);
void mark_position(int line, int column, char board[][BOARD_MAX], char player,
                   int current_mboard_coord[][COORD_DIMENSION],
                   char macro_board[][BOARD_MAX], int n, char *winner,
                   int generated, int coeffs[][PLAYERS]);
int execute_move(char turn, char player, int x, int y,
                 int coord[][CORN_NUMBER][COORD_DIMENSION],
                 char board[][BOARD_MAX], int n, char macro_board[][BOARD_MAX],
                 char *winner, int coeffs[][PLAYERS]);
void task2(int n, char macro_board[][BOARD_MAX], char *winner);
void task3(int coeffs[][PLAYERS]);
void read_data();
#endif
