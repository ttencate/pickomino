#ifndef GAME_H
#define GAME_H

#include "dice.h"

extern const int NUM_DICE;

class Game {
  public:
    Game(int wormsToLose) :
      wormsToLose(wormsToLose)
    {
    }

    int wormsForScore(Score score) const {
      // Pretending for now that all tiles are up for grabs on the table.
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
      return -wormsToLose;
    }

    bool canQuit(Dice taken) {
      return taken.contains(DieSide::WORM);
    }

    bool canRoll(Dice taken) {
      return taken.count() < NUM_DICE;
    }

  private:
    int wormsToLose;
};

#endif
