#ifndef HUMAN_H
#define HUMAN_H

#include "dice.h"
#include "game.h"

class Human : public Strategy {
  public:
    Human(std::string name) :
      Strategy(name)
    {}

    void prepareTurn(Game const &game) override;
    bool chooseWhetherToRoll(Game const &game, Dice const &taken) override;
    DieSide const *chooseSideToTake(Game const &game, Dice const &taken, Dice const &roll) override;
};

#endif
