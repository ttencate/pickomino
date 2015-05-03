#include "human.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <string>

using namespace std;

void Human::prepareTurn(Game const &) {
}

bool Human::chooseWhetherToRoll(Game const &, Dice const &) {
  cout << "Roll again? [Y/n] ";
  string answer;
  do {
    getline(cin, answer);
  } while (answer != "" && answer != "y" && answer != "n");
  return answer == "" || answer == "y";
}

DieSide const *Human::chooseSideToTake(Game const &, Dice const &taken, Dice const &roll) {
  cout << "Which side to take? [";
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      cout << side->toString();
    }
  }
  cout << "] ";
  string answer;
  DieSide const *side;
  do {
    getline(cin, answer);
    side = DieSide::fromString(answer);
  } while (!side || !roll.contains(side) || taken.contains(side));
  return side;
}
