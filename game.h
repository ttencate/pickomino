#ifndef GAME_H
#define GAME_H

#include "dice.h"

class GameState {
  public:
    GameState(int wormsToLose) :
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

  private:
    int wormsToLose;
};

#endif
