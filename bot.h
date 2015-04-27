#ifndef BOT_H
#define BOT_H

#include "dice.h"
#include "game.h"

typedef double ExpectedWorms;

class Strategy {
  public:
    Strategy() {}

    virtual bool chooseWhetherToRoll(Dice const &taken) = 0;
    virtual DieSide const *chooseSideToTake(Dice const &taken, Dice const &roll) = 0;

  private:
    Strategy(Strategy const &) = delete;
    Strategy &operator=(Strategy const &) = delete;
};

class Bot : public Strategy {
  public:
    Bot(Game *game) :
      m_game(game)
    {}

    bool chooseWhetherToRoll(Dice const &taken);
    DieSide const *chooseSideToTake(Dice const &taken, Dice const &roll);

  private:
    Game *const m_game;

    ExpectedWorms expectedWormsWhenTaking(Dice taken, Dice const &roll, DieSide const *side);
    ExpectedWorms expectedWormsWhenRolled(Dice taken, Dice const &roll);
    ExpectedWorms expectedWormsWhenRolling(Dice taken);
    ExpectedWorms expectedWormsWhenQuitting(Dice taken);
};

#endif
