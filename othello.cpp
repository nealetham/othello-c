#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cstring>
#include <algorithm>
#include <iostream>
#include "othello-naive-ai.h"

#define ROWS 8
#define COLUMNS 8
#define DIRECTIONS 8
#define BLACK_DISK 'O'
#define WHITE_DISK 'X'
#define EMPTY_CELL ' '
#define PLAYABLE_CELL '*'

enum CELL_STATUS
{
  BLACK = 0,
  WHITE = 1,
  EMPTY = 2,
  PLAYABLE = 3
};

int board[ROWS][COLUMNS];
int scores[2];
int current_player;
int move_direction[ROWS][COLUMNS][DIRECTIONS];
bool game_style;

std::vector<std::array<int, 2>> player_moves;
std::array<int, 2> previous_move = {-1, -1};

void setup()
{
  memset(board, EMPTY, sizeof(board));
  board[3][3] = board[4][4] = BLACK;
  board[3][4] = board[4][3] = WHITE;
  scores[BLACK] = scores[WHITE] = 2;
  current_player = BLACK;
}

void execute_move(int row, int col)
{
  int opponent = 1 - current_player;
  board[row][col] = current_player;
  int score_gain = 1;

  for (int dir = 0; dir < DIRECTIONS; dir++)
  {
    int action = move_direction[row][col][dir];
    if (!action)
    {
      continue; // not valid in any direction
    }

    int row_dir = playable_directions[dir][0];
    int col_dir = playable_directions[dir][1];

    int row_temp = row + row_dir;
    int col_temp = col + col_dir;

    while (board[row_temp][col_temp] == opponent)
    {
      board[row_temp][col_temp] = current_player;
      score_gain += 1;
      row_temp += row_dir;
      col_temp += col_dir;
    }
  }
  scores[current_player] += score_gain;
  scores[opponent] -= (score_gain - 1);
}

bool is_valid_position(int row, int col)
{
  return !(row < 0 || row >= 8 || col < 0 || col >= 8);
}

int distance(int src_row, int src_col, int dst_row, int dst_col)
{
  int d_row = abs(src_row - dst_row);
  int d_col = abs(src_col - dst_col);
  return std::max(d_row, d_col);
}

bool in_player_moves(int row, int col)
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

int is_possible_move(int row, int col, int opponent)
{
  if (board[row][col] == opponent || board[row][col] == current_player)
  {
    return false;
  }

  memset(move_direction[row][col], 0, 1);
  bool playable = false;

  for (int dir = 0; dir < DIRECTIONS; dir++)
  {
    int row_dir = playable_directions[dir][0];
    int col_dir = playable_directions[dir][1];

    int row_temp = row + row_dir;
    int col_temp = col + col_dir;

    while (is_valid_position(row_temp, col_temp) && board[row_temp][col_temp] == opponent)
    {
      row_temp += row_dir;
      col_temp += col_dir;
    }

    if (is_valid_position(row_temp, col_temp) && board[row_temp][col_temp] == current_player && distance(row, col, row_temp, col_temp) > 1)
    {
      move_direction[row][col][dir] = 1;
      playable = true;
    }
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
      bool playable = is_possible_move(row, col, opponent);
      if (!playable)
      {
        continue;
      }
      possible_moves.push_back(std::array<int, 2>{row, col});
    }
  }
  player_moves = possible_moves;
}

void prompt_move()
{
  int row, col;

  if (game_style && current_player == 1)
  {
    std::array<int, 2> best_move = evaluate_moves(current_player, board, player_moves, move_direction);
    execute_move(best_move[0], best_move[1]);
    previous_move = {best_move[0],
                     best_move[1]};
  }
  else
  {
    while (1)
    {
      std::cout << "Previous move: [" << previous_move[0] << ',' << previous_move[1] << ']' << '\n';
      std::cout << "Enter the row and column (e.g. 2 0)\n";
      std::cin >> row >> col;
      if (in_player_moves(row, col))
      {
        execute_move(row, col);
        previous_move = {row, col};
        break;
      }
      std::cout << "You can't make that move. Please try again!\n\n";
    }
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

  std::cout << "\n#############################\n";
  std::cout << "          PLAYER " << current_player;
  std::cout << "\n#############################\n\n";
  std::cout << "Player O: " << scores[0];
  std::cout << "     ";
  std::cout << "Player X: " << scores[1] << "\n\n";
  std::cout << "    0  1  2  3  4  5  6  7\n";
  std::cout << "   _________________________\n";

  for (int row = 0; row < ROWS; row++)
  {
    std::cout << row << " |";
    for (int col = 0; col < COLUMNS; col++)
    {
      int cell_status = marked_board[row][col];

      switch (cell_status)
      {
      case CELL_STATUS::WHITE:
        std::cout << " " << WHITE_DISK << " ";
        continue;
      case CELL_STATUS::BLACK:
        std::cout << " " << BLACK_DISK << " ";
        continue;
      case CELL_STATUS::PLAYABLE:
        std::cout << " " << PLAYABLE_CELL << " ";
        continue;
      default:
        std::cout << " " << EMPTY_CELL << " ";
      }
    }
    std::cout << "|\n";
  }
  std::cout << "  |________________________|\n\n";
}

void next_player()
{
  current_player = 1 - current_player;
}

void display_winner()
{
  std::cout << "\n#############################\n";
  std::cout << "         Final Score ";
  std::cout << "\n#############################\n";
  std::cout << "Player " << BLACK_DISK << ": " << scores[BLACK];
  std::cout << "     ";
  std::cout << "Player " << WHITE_DISK << ": " << scores[WHITE] << "\n";

  if (scores[WHITE] > scores[BLACK])
  {
    std::cout << "Player " << WHITE_DISK << " wins!";
  }
  else if (scores[WHITE] < scores[BLACK])
  {
    std::cout << "Player " << BLACK_DISK << " wins!";
  }
  else
    std::cout << "\nGame was drawn!\n";
}

bool prompt_game_style()
{
  bool style_ai;
  std::cout << "Type 0 to play locally with a friend.\n";
  std::cout << "Type 1 to play against a machine.\n";
  std::cin >> style_ai;
  return style_ai;
}

int main()
{
  setup();
  game_style = prompt_game_style();
  int game_end = 0;
  int consecutive_skips = 0;

  while (!game_end)
  {
    std::cout << "\033c";
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
    prompt_move();
    consecutive_skips = 0;
    next_player();
  }
  display_winner();
}
