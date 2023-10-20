#include <vector>
#include <array>
#include "othello-naive-ai.h"

int playable_directions[8][2] = {
    {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1}};

int evaluate_move(int current_player, int row, int col, int board[8][8], int move_direction[8][8][8])
{
  int opponent = 1 - current_player;
  int score_gain = 1;

  for (int dir = 0; dir < 8; dir++)
  {
    int action = move_direction[row][col][dir];
    if (!action)
    {
      continue;
    }

    int row_dir = playable_directions[dir][0];
    int col_dir = playable_directions[dir][1];

    int row_temp = row + row_dir;
    int col_temp = col + col_dir;

    while (board[row_temp][col_temp] == opponent)
    {
      score_gain += 1;
      row_temp += row_dir;
      col_temp += col_dir;
    }
  }
  return score_gain;
}

std::array<int, 2> evaluate_moves(int current_player, int board[8][8], std::vector<std::array<int, 2>> player_moves, int move_direction[8][8][8])
{
  std::array<int, 2> best_move;
  int best_value = 0;
  for (const std::array<int, 2> move : player_moves)
  {
    int curr_value = evaluate_move(current_player, move[0], move[1], board, move_direction);
    if (curr_value > best_value)
    {
      best_move = move;
    }
  }
  return best_move;
}
