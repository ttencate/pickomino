#include "dice.h"

using namespace std;

DieSide const DieSide::S_ONE(0, 1, "1", '1');
DieSide const DieSide::S_TWO(1, 2, "2", '2');
DieSide const DieSide::S_THREE(2, 3, "3", '3');
DieSide const DieSide::S_FOUR(3, 4, "4", '4');
DieSide const DieSide::S_FIVE(4, 5, "5", '5');
DieSide const DieSide::S_WORM(5, 5, "w", 'w');

DieSide const *const DieSide::ONE = &DieSide::S_ONE;
DieSide const *const DieSide::TWO = &DieSide::S_TWO;
DieSide const *const DieSide::THREE = &DieSide::S_THREE;
DieSide const *const DieSide::FOUR = &DieSide::S_FOUR;
DieSide const *const DieSide::FIVE = &DieSide::S_FIVE;
DieSide const *const DieSide::WORM = &DieSide::S_WORM;

vector<DieSide const *> const DieSide::ALL = {
  &DieSide::S_ONE, &DieSide::S_TWO, &DieSide::S_THREE, &DieSide::S_FOUR, &DieSide::S_FIVE, &DieSide::S_WORM
};

ostream &operator<<(ostream &out, Dice const &dice) {
  for (DieSide const *side : DieSide::ALL) {
    for (int j = 0; j < dice[side]; ++j) {
      out << side->toString();
    }
  }
  return out;
}

istream &operator>>(istream &in, Dice &dice) {
  string input;
  for (DieSide const *side : DieSide::ALL) {
    dice[side] = 0;
  }
  if (in >> input) {
    for (unsigned i = 0; i < input.size(); ++i) {
      char c = input[i];
      for (DieSide const *side : DieSide::ALL) {
        if (c == side->inputChar()) {
          dice[side]++;
          break;
        }
      }
    }
  }
  return in;
}
