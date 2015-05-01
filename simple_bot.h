#ifndef SIMPLE_BOT_H
#define SIMPLE_BOT_H

#include "dice.h"
#include "game.h"

class SimpleBot : public Strategy {
  public:
    SimpleBot(std::string name) :
      Strategy(name)
    {}

    void prepareTurn(Game const &game) override;
    bool chooseWhetherToRoll(Game const &game, Dice const &taken) override;
    DieSide const *chooseSideToTake(Game const &game, Dice const &taken, Dice const &roll) override;
};

#endif

