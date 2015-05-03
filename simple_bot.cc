#include "simple_bot.h"
#include "roll.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

void SimpleBot::prepareTurn(Game const &) {
}

bool SimpleBot::chooseWhetherToRoll(Game const &game, Dice const &taken) {
  Tile take = game.stealableTile(this, taken);
  if (!take.valid()) {
    take = game.bestRemainingTile(taken);
  }
  if (!take.valid()) {
    cout << "\"No tile to take yet.\"\n";
    return true;
  }
  cout << "\"I can take a tile.\"\n";
  return false;
}

DieSide const *SimpleBot::chooseSideToTake(Game const &, Dice const &taken, Dice const &roll) {
  Score maxScore = 0;
  DieSide const *maxSide = nullptr;
  for (DieSide const *side : {DieSide::WORM, DieSide::FIVE, DieSide::FOUR, DieSide::THREE, DieSide::TWO, DieSide::ONE}) {
    if (roll.contains(side) && !taken.contains(side)) {
      if (side == DieSide::WORM && taken.sideCount() >= 2) {
        return side;
      }
      Score score = side->score() * roll[side];
      if (score > maxScore) {
        maxScore = score;
        maxSide = side;
      }
    }
  }
  assert(maxSide);
  cout << "\"Highest value side is " << maxSide->toString() << ".\"\n";
  return maxSide;
}
