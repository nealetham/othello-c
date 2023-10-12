#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <vector>
#include <array>
#include <iostream>

#define ROWS 8
#define COLUMNS 8
#define BLACK 0
#define WHITE 1
#define EMPTY 2
#define PLAYABLE 3

#define BLACK_DISC 'O'
#define WHITE_DISC 'X'
#define EMPTY_CELL ' '
#define PLAYABLE_CELL '*'

int board[ROWS][COLUMNS];
int playable_direction[ROWS][COLUMNS][8];
int scores[2];
std::vector<std::array<int, 2>> player_moves;
int current_player;

void init_board()
{
  memset(board, EMPTY, sizeof(board));
  for (int row = 0; row < ROWS; row++)
  {
    for (int col = 0; col < COLUMNS; col++)
    {
      board[row][col] = EMPTY;
    }
  }
  board[3][3] = BLACK;
  board[4][4] = BLACK;
  board[3][4] = WHITE;
  board[4][3] = WHITE;
  scores[BLACK] = 2;
  scores[WHITE] = 2;
  current_player = BLACK;
}

void place_disc(int row, int col)
{
  int opponent = 1 - current_player;
  board[row][col] = current_player;
  int score_gain = 1;

  for (int cell = 0; cell < 8; cell++)
  {
    int action = playable_direction[row][col][cell];
    if (!action)
    {
      continue; // not valid in any direction
    }
    int row_ul, col_ul, row_up, row_ur, col_ur, col_left, col_right, row_ll, col_ll, row_down, row_lr, col_lr;
    switch (cell)
    {
    case 0: // Upper Left Diagonal
      row_ul = row - 1;
      col_ul = col - 1;
      while (board[row_ul][col_ul] == opponent)
      {
        board[row_ul][col_ul] = current_player;
        score_gain += 1;
        row_ul -= 1;
        col_ul -= 1;
      }
      break;

    case 1: // Top
      printf("[%d, %d] Top\n", row, col);
      row_up = row - 1;
      while (board[row_up][col] == opponent)
      {
        board[row_up][col] = current_player;
        score_gain += 1;
        row_up -= 1;
      }
      break;

    case 2: // Upper Right Diagonal
      printf("[%d, %d] URD\n", row, col);
      row_ur = row - 1;
      col_ur = col + 1;
      while (board[row_ur][col_ur] == opponent)
      {
        board[row_ur][col_ur] = current_player;
        score_gain += 1;
        row_ur -= 1;
        col_ur += 1;
      }
      break;

    case 3: // Left
      printf("[%d, %d] Left\n", row, col);
      col_left = col - 1;
      while (board[row][col_left] == opponent)
      {
        board[row][col_left] = current_player;
        score_gain += 1;
        col_left -= 1;
      }
      break;

    case 4: // Right
      printf("[%d, %d] Right\n", row, col);
      col_right = col + 1;
      while (board[row][col_right] == opponent)
      {
        board[row][col_right] = current_player;
        score_gain += 1;
        col_right += 1;
      }
      break;

    case 5: // Lower Left Diagonal
      printf("[%d, %d] LLD\n", row, col);
      row_ll = row + 1;
      col_ll = col - 1;
      while (board[row_ll][col_ll] == opponent)
      {
        board[row_ll][col_ll] = current_player;
        score_gain += 1;
        row_ll += 1;
        col_ll -= 1;
      }
      break;

    case 6: // Down
      printf("[%d, %d] Down\n", row, col);
      row_down = row + 1;
      while (board[row_down][col] == opponent)
      {
        board[row_down][col] = current_player;
        score_gain += 1;
        row_down += 1;
      }
      break;

    case 7: // Lower Right Diagonal
      printf("[%d, %d] LRD\n", row, col);
      row_lr = row + 1;
      col_lr = col + 1;
      while (board[row_lr][col_lr] == opponent)
      {
        board[row_lr][col_lr] = current_player;
        score_gain += 1;
        row_lr += 1;
        col_lr += 1;
      }
    }
  }
  scores[current_player] += score_gain;
  scores[opponent] -= (score_gain - 1);
}

bool is_valid_position(int row, int col)
{
  if (row < 0 || row >= 8 || col < 0 || col >= 8)
  {
    return false;
  }
  return true;
}

int distance(int src_row, int src_col, int dst_row, int dst_col)
{
  int d_row = abs(src_row - dst_row);
  int d_col = abs(src_col - dst_col);
  return std::max(d_row, d_col);
}

bool in_move_set(int row, int col)
{
  std::array<int, 2> move{row, col};
  auto iter = std::find(player_moves.begin(), player_moves.end(), move);
  if (iter != player_moves.end())
  {
    player_moves.erase(iter);
    return true;
  }

  return false;
}

int is_playable(int row, int col, int opponent)
{
  memset(playable_direction[row][col], 0, 1);
  bool playable = false;

  // Upper Left Diagonal
  int row_ul = row - 1;
  int col_ul = col - 1;
  while (is_valid_position(row_ul, col_ul) && board[row_ul][col_ul] == opponent)
  {
    row_ul -= 1;
    col_ul -= 1;
  }
  if (is_valid_position(row_ul, col_ul) && board[row_ul][col_ul] == current_player && distance(row, col, row_ul, col_ul) > 1)
  {
    playable_direction[row][col][0] = 1;
    playable = true;
  }

  // Top
  int row_up = row - 1;
  while (is_valid_position(row_up, col) && board[row_up][col] == opponent)
  {
    row_up -= 1;
  }
  if (is_valid_position(row_up, col) && board[row_up][col] == current_player && distance(row, col, row_up, col) > 1)
  {
    playable_direction[row][col][1] = 1;
    playable = true;
  }

  // Upper Right Diagonal
  int row_ur = row - 1;
  int col_ur = col + 1;
  while (is_valid_position(row_ur, col_ur) && board[row_ur][col_ur] == opponent)
  {
    row_ur -= 1;
    col_ur += 1;
  }
  if (is_valid_position(row_ur, col_ur) && board[row_ur][col_ur] == current_player && distance(row, col, row_ur, col_ur) > 1)
  {
    playable_direction[row][col][2] = 1;
    playable = true;
  }

  // Left
  int col_left = col - 1;
  while (is_valid_position(row, col_left) && board[row][col_left] == opponent)
  {
    col_left -= 1;
  }
  if (is_valid_position(row, col_left) && board[row][col_left] == current_player && distance(row, col, row, col_left) > 1)
  {
    playable_direction[row][col][3] = 1;
    playable = true;
  }

  // Right
  int col_right = col + 1;
  while (is_valid_position(row, col_right) && board[row][col_right] == opponent)
  {
    col_right += 1;
  }
  if (is_valid_position(row, col_right) && board[row][col_right] == current_player && distance(row, col, row, col_right) > 1)
  {
    playable_direction[row][col][4] = 1;
    playable = true;
  }

  // Lower Left Diagonal
  int row_ll = row + 1;
  int col_ll = col - 1;
  while (is_valid_position(row_ll, col_ll) && board[row_ll][col_ll] == opponent)
  {
    row_ll += 1;
    col_ll -= 1;
  }
  if (is_valid_position(row_ll, col_ll) && board[row_ll][col_ll] == current_player && distance(row, col, row_ll, col_ll) > 1)
  {
    playable_direction[row][col][5] = 1;
    playable = true;
  }

  // Down
  int row_down = row + 1;
  while (is_valid_position(row_down, col) && board[row_down][col] == opponent)
  {
    row_down += 1;
  }
  if (is_valid_position(row_down, col) && board[row_down][col] == current_player && distance(row, col, row_down, col) > 1)
  {
    playable_direction[row][col][6] = 1;
    playable = true;
  }

  // Lower Right Diagonal
  int row_lr = row + 1;
  int col_lr = col + 1;
  while (is_valid_position(row_lr, col_lr) && board[row_lr][col_lr] == opponent)
  {
    row_lr += 1;
    col_lr += 1;
  }
  if (is_valid_position(row_lr, col_lr) && board[row_lr][col_lr] == current_player && distance(row, col, row_lr, col_lr) > 1)
  {
    playable_direction[row][col][7] = 1;
    playable = true;
  }

  return playable;
}

void get_possible_moves()
{
  int opponent = 1 - current_player;
  std::vector<std::array<int, 2>> possible_moves;
  for (int row = 0; row < ROWS; row++)
  {
    for (int col = 0; col < COLUMNS; col++)
    {
      if (board[row][col] == opponent || board[row][col] == current_player)
      {
        continue; // another disc on this position
      }

      bool playable = is_playable(row, col, opponent);
      if (!playable)
      {
        continue;
      }

      possible_moves.push_back(std::array<int, 2>{row, col});
    }
  }
  player_moves = possible_moves;
}

void make_move()
{
  int row;
  int col;

  while (true)
  {
    printf("Enter row [0-7] and column [0-7] separated by a single space (e.g. 2 0).\n");
    std::cin >> row >> col;

    if (in_move_set(row, col))
    {
      place_disc(row, col);
      break;
    }

    printf("Move was not valid. Please try again!\n\n");
    continue;
  }
}

void display_board()
{

  int marked_board[ROWS][COLUMNS];
  std::copy(&board[0][0], &board[0][0] + ROWS * COLUMNS, &marked_board[0][0]);

  for (const std::array<int, 2> elem : player_moves)
  {
    marked_board[elem[0]][elem[1]] = PLAYABLE;
  }

  printf("\n#############################\n");
  printf("####    PLAYER %d TURN    ####\n", current_player);
  printf("#############################\n");
  printf("P0: %d    P1: %d\n\n", scores[0], scores[1]);
  printf("    0  1  2  3  4  5  6  7\n");
  printf("   _________________________\n");

  for (int row = 0; row < ROWS; row++)
  {
    printf("%d |", row);
    for (int col = 0; col < COLUMNS; col++)
    {
      int cell = marked_board[row][col];

      switch (cell)
      {
      case WHITE:
        printf(" %c ", WHITE_DISC);
        continue;
      case BLACK:
        printf(" %c ", BLACK_DISC);
        continue;
      case EMPTY:
        printf(" %c ", EMPTY_CELL);
        continue;
      default:
        printf(" %c ", PLAYABLE_CELL);
      }
    }
    printf("|\n");
  }
  printf("  |________________________|\n");
  printf("\n");
}

void next_player()
{
  current_player = 1 - current_player;
}

void display_winner()
{
  printf("\n#############################\n");
  printf("Final score:\n%c: %d %c: %d\n", BLACK_DISC, scores[BLACK], WHITE_DISC, scores[WHITE]);
  if (scores[WHITE] > scores[BLACK])
    printf("%c wins.\n", WHITE_DISC);
  else if (scores[WHITE] < scores[BLACK])
    printf("%c wins.\n", BLACK_DISC);
  else
    printf("Draw game.\n");
  printf("\n#############################\n");
}

int main()
{
  init_board();
  int game_end = 0;
  int consecutive_skips = 0;

  while (!game_end)
  {
    get_possible_moves();
    if (!player_moves.size())
    {
      consecutive_skips += 1;
      if (consecutive_skips == 2)
      {
        game_end = 1;
      }
      next_player();
      continue;
    }
    display_board();
    make_move();
    consecutive_skips = 0;
    next_player();
  }
  display_winner();
}
