#include <vector>
#include <array>

extern int playable_directions[8][2];

int evaluate_move(int, int, int, int[8][8], int[8][8][8]);
std::array<int, 2> evaluate_moves(int, int[8][8], std::vector<std::array<int, 2>>, int[8][8][8]);
