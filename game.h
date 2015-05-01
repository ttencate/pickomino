#ifndef GAME_H
#define GAME_H

#include "dice.h"

extern const int NUM_DICE;

class Game {
  public:
    Game() :
      m_wormsToLose(0),
      m_lowestTile(21)
    {
    }

    int wormsForScore(Score score) const {
      if (score < m_lowestTile) {
        return wormsForDeath();
      }
      if (score < 21) {
        return wormsForDeath();
      } else if (score < 25) {
        return 1;
      } else if (score < 29) {
        return 2;
      } else if (score < 33) {
        return 3;
      } else {
        return 4;
      }
    }

    int wormsForDeath() const {
      return -m_wormsToLose;
    }

    bool canRoll(Dice taken) {
      return taken.count() < NUM_DICE;
    }

    bool canTakeAny(Dice taken, Dice roll) {
      for (DieSide const *side : DieSide::ALL) {
        if (roll.contains(side) && !taken.contains(side)) {
          return true;
        }
      }
      return false;
    }

  private:
    int m_wormsToLose;
    int m_lowestTile;
};

#endif
