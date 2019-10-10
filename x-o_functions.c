// Copyright 2018 Samoilescu Sebastian
#include "./x-o_functions.h"

// Consideram intreg boardul ca fiind "empty"
void initialize_board(int n, char board[][BOARD_MAX]) {
  for (int i = 0; i < n * n; ++i) {
    for (int j = 0; j < n * n; ++j) {
      board[i][j] = 'E';
    }
  }
}

// Consideram ca in fiecare mini-board e remiza
void initialize_macro_board(int n, char macro_board[][BOARD_MAX]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j)
      macro_board[i][j] = '-';
  }
}

// Afisam macro-board-ul
void print_macro_board(int n, char macro_board[][BOARD_MAX]) {
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      printf("%c", macro_board[i][j]);
    }
    printf("\n");
  }
}

// Pentru cele n * n mini-boarduri salvam coordonatele acestora intr-un tablou
// tri dimensional(vectori de matrici 2 X 2).Unde prin coordonate intelegem
// coordonatele punctului stanga sus si dreapta jos
void calculate_coord_of_boards(
    int n, int coord[BOARD_MAX][CORN_NUMBER][COORD_DIMENSION]) {
  for (int i = 0; i < n * n; ++i) {
    int line = i / n;
    int column = i % n;
    coord[i][0][0] = line * n;
    coord[i][0][1] = column * n;
    coord[i][1][0] = (line + 1) * n - 1;
    coord[i][1][1] = (column + 1) * n - 1;
  }
}

// Schimba randul
char change_turn(char turn) {
  if (turn == 'X') {
    return '0';
  }
  return 'X';
}

// Verifica daca casuta din board e libera pentru a fi marcata
int is_not_empty(int x, int y, char board[][BOARD_MAX]) {
  return !(board[x][y] == 'E');
}

// Se obtine mini-boardul ce contine punctul de cord (x,y)
// Se obtine "INVALID" atunci cand indexi dati sunt invalizi
int get_mini_board(int x, int y, int n) {
  if (x >= n * n || y >= n * n || x < 0 || y < 0) {
    return INVALID;
  }
  short int mini_board = (y / n) + (x / n) * n;
  return mini_board;
}

// Se verifica daca un player a realizat o linie completa
int verify_line(int line,
                int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                char board[][BOARD_MAX], char player) {
  for (int j = current_mboard_coord[0][1]; j <= current_mboard_coord[1][1];
       ++j) {
    if (board[line][j] != player) {
      return 0;
    }
  }
  return 1;
}

// Se verifica daca un player a realizat o coloana completa
int verify_column(int column,
                  int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                  char board[][BOARD_MAX], char player) {
  for (int j = current_mboard_coord[0][0]; j <= current_mboard_coord[1][0];
       ++j) {
    if (board[j][column] != player) {
      return 0;
    }
  }
  return 1;
}

// Se verifica daca un player a relizat complet diagonala principala
int verify_main_diagonal(int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                         char board[][BOARD_MAX], char player) {
  int n = (current_mboard_coord[1][0] - current_mboard_coord[0][0]);
  for (int i = 0; i <= n; ++i) {
    if (board[i + current_mboard_coord[0][0]][i + current_mboard_coord[0][1]] !=
        player) {
      return 0;
    }
  }
  return 1;
}

// Se verifica daca un player a realizat complet diagonala secundara
int verify_second_diagonal(
    int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
    char board[][BOARD_MAX], char player) {
  int n = (current_mboard_coord[1][0] - current_mboard_coord[0][0]);
  for (int i = 0; i <= n; ++i) {
    if (board[i + current_mboard_coord[0][0]]
             [n - i + current_mboard_coord[0][1]] != player) {
      return 0;
    }
  }
  return 1;
}

// Se verifica mini boardul daca are o linie, coloana, diagonala completa
// Verficare se face cu functiile explicate mai sus.
// Verificam mini-boardul de ficare data cand a s-a marcat o noua pozitie.
int verify_mini_board(int line, int column,
                      int current_mboard_coord[CORN_NUMBER][COORD_DIMENSION],
                      char board[][BOARD_MAX], char player) {
  int main_diagonal = 0, second_diagonal = 0;
  if (line - current_mboard_coord[0][0] ==
      column - current_mboard_coord[0][1]) {
    main_diagonal = verify_main_diagonal(current_mboard_coord, board, player);
  }
  if (column - current_mboard_coord[0][1] ==
      (current_mboard_coord[1][0] - line)) {
    second_diagonal =
        verify_second_diagonal(current_mboard_coord, board, player);
  }
  return (verify_line(line, current_mboard_coord, board, player) ||
          verify_column(column, current_mboard_coord, board, player) ||
          main_diagonal || second_diagonal);
}

// Se verifica daca a fost castigat mini-boardul dat prin "line" si "column".
// In cazul in care a fost catigat pentru prima data vom marca casuta asociata
// mini-board-ului in macroboard.
int update_macro_board(int line, int column, char board[][BOARD_MAX],
                       char player, int current_mboard_coord[][COORD_DIMENSION],
                       char macro_board[][BOARD_MAX], int n) {
  int mini_board = get_mini_board(line, column, n);
  int line_mini = mini_board / n;
  int column_mini = mini_board % n;
  if (macro_board[line_mini][column_mini] == '-' &&
      verify_mini_board(line, column, current_mboard_coord, board, player)) {
    macro_board[mini_board / n][mini_board % n] = player;
    return 1;
  }
  return 0;
}

// Updatam coeficientii ajutatori pentru a calcula coeficientul de atentie.
// Type = 1 e atunci cand trebuie sa crestem numarul de mini_boarduri castigate
// Type = 2 e atunci cand trebuie sa crestem numarul de runde jucate de 'player'
void update_coeffs(int generated, int coeffs[][PLAYERS], char player,
                   int type) {
  if (generated == 1 && type == 1) {
    return;
  }
  int player_index = 0;
  if (player == '0') {
    player_index = 1;
  }
  coeffs[player_index][type]++;
}

// Functia returneaza dupa algoritmul prezentat in enunt coordonatele
// unei pozitii libere, parcurgand diagonalele board-ului
int get_random_position(char board[][BOARD_MAX], int n,
                        int generated_position[]) {
  for (int i = 0; i < n * n; ++i) {
    for (int j = 0; j < n * n - i; ++j) {
      if (board[j][j + i] == 'E') {
        generated_position[0] = j;
        generated_position[1] = j + i;
        return 1;
      }
    }
    for (int j = 0; j < n * n - i; ++j) {
      if (board[j + i][j] == 'E') {
        generated_position[0] = j + i;
        generated_position[1] = j;
        return 1;
      }
    }
  }
  return 0;
}

// Verifica daca s-a format o linie, coloana, diagonala completain macroboard
// Variabila "winner" retine starea macroboard-ului la un moment.
// Codficarea se face astfel D - draw, 0,X - castigat de 0 SAU X,'-' - draw
// 0bs: Diferenta dintre 'D' si '-' este data de faptul ca D apare cand amandoi
// jucatorii au castigat macroboard-ul, iar '-' atunci cand niciunul nu l-a
// castigat
void chose_winner(int line, int column, char player,
                  char macro_board[][BOARD_MAX], int n, char *winner) {
  if (*winner == 'D') {
    return;
  }
  int macro_board_coord[CORN_NUMBER][COORD_DIMENSION] = {{0, 0},
                                                         {n - 1, n - 1}};
  int mini_board = get_mini_board(line, column, n);
  int line_mini = mini_board / n;
  int column_mini = mini_board % n;
  if (verify_mini_board(line_mini, column_mini, macro_board_coord, macro_board,
                        player)) {
    if (*winner != player && *winner != '-') {
      *winner = 'D';
    } else {
      *winner = player;
    }
  }
}

// Marcam pozitia data prin "line" si "column" si lansam procesele legate de
// marcarea unei pozitii.
void mark_position(int line, int column, char board[][BOARD_MAX], char player,
                   int current_mboard_coord[][COORD_DIMENSION],
                   char macro_board[][BOARD_MAX], int n, char *winner,
                   int generated, int coeffs[][PLAYERS]) {
  board[line][column] = player;
  update_coeffs(generated, coeffs, player, 0);
  if (update_macro_board(line, column, board, player, current_mboard_coord,
                         macro_board, n)) {
    update_coeffs(generated, coeffs, player, 1);
    chose_winner(line, column, player, macro_board, n, winner);
  }
}

// Se incearca marcarea pozitiei date de (x,y)
int execute_move(char turn, char player, int x, int y,
                 int coord[][CORN_NUMBER][COORD_DIMENSION],
                 char board[][BOARD_MAX], int n, char macro_board[][BOARD_MAX],
                 char *winner, int coeffs[][PLAYERS]) {
  if (turn != player) {
    printf("NOT YOUR TURN\n");
    return 2;
  }
  int generated_position[2] = {x, y};
  int mini_board = get_mini_board(x, y, n);
  int result = 1;
  int generated = 0;
  // Verificam daca mutarea facuta de player este valida.In caz negativ,
  // generam o pozitie valida dupa algoritmul din enunt si marcam acest fapt
  if (mini_board == INVALID) {
    printf("INVALID INDEX\n");
    result = get_random_position(board, n, generated_position);
    generated = 1;
  } else if (is_not_empty(x, y, board)) {
    printf("NOT AN EMPTY CELL\n");
    result = get_random_position(board, n, generated_position);
    generated = 1;
  }

  // Daca s-a generat/introdus o pozitie valida o marcam,altfel afisam
  // "full board"
  if (result == 1) {
    x = generated_position[0];
    y = generated_position[1];
    mini_board = get_mini_board(x, y, n);
    mark_position(x, y, board, player, coord[mini_board], macro_board, n,
                  winner, generated, coeffs);
  } else if (result == 0) {
    printf("FULL BOARD\n");
  }
  return result;
}

// printam macro-board-ul si afisam mesajul corespunzator
void task2(int n, char macro_board[][BOARD_MAX], char *winner) {
  print_macro_board(n, macro_board);
  if (*winner == '-' || *winner == 'D') {
    printf("Draw again! Let's play darts!\n");
  } else {
    printf("%c won\n", *winner);
  }
}

// Calculam coeficientii de atentie pe baza matricii construite in functia
// "update_coef"
void task3(int coeffs[][PLAYERS]) {
  if (coeffs[0][0] == 0) {
    printf("X N/A\n");
  } else {
    double attention = (1.0 * coeffs[0][1]) / coeffs[0][0];
    printf("X %.10lf\n", attention);
  }
  if (coeffs[1][0] == 0) {
    printf("0 N/A\n");
  } else {
    double attention = (1.0 * coeffs[1][1]) / coeffs[1][0];
    printf("0 %.10lf\n", attention);
  }
}
// Citim datele de intrare
void read_data() {
  char board[BOARD_MAX][BOARD_MAX], player, turn = 'X',
       macro_board[BOARD_MAX][BOARD_MAX];
  int n, m, x = 0, y = 0, coord[BOARD_MAX][CORN_NUMBER][COORD_DIMENSION];
  // Folosim variabila pentru a retine rezultatul functiei execute_move
  // executing = {0,1,2},unde 0 - boardul este plin,nu se mai exec mutari
  // 1 - se exec mutari in continuare,
  // 2 - un jucator a jucat cand nu era randul sau
  int executing = 1;
  char winner = {'-'};
  int coeffs[PLAYERS][PLAYERS] = {{0}};
  scanf("%d", &n);
  scanf("%d", &m);
  initialize_board(n, board);
  initialize_macro_board(n, macro_board);
  calculate_coord_of_boards(n, coord);
  for (int i = 1; i <= m; ++i) {
    scanf("%c", &player);
    scanf("%c", &player);
    scanf("%d", &x);
    scanf("%d", &y);
    if (executing) {
      executing = execute_move(turn, player, x, y, coord, board, n, macro_board,
                               &winner, coeffs);
    }
    if (executing == 1) {
      turn = change_turn(turn);
    }
  }
  task2(n, macro_board, &winner);
  task3(coeffs);
}
