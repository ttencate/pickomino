#ifndef BOT_H
#define BOT_H

#include <unordered_map>

#include "dice.h"
#include "game.h"

typedef double ExpectedWorms;

class Strategy {
  public:
    Strategy() {}

    virtual void prepareTurn() = 0;
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

    virtual void prepareTurn();
    virtual bool chooseWhetherToRoll(Dice const &taken);
    virtual DieSide const *chooseSideToTake(Dice const &taken, Dice const &roll);

  private:
    Game *const m_game;

    std::unordered_map<Dice, ExpectedWorms> m_expectedWhenRolling;

    ExpectedWorms expectedWormsWhenTaking(Dice taken, Dice const &roll, DieSide const *side);
    ExpectedWorms expectedWormsWhenRolled(Dice taken, Dice const &roll);
    ExpectedWorms expectedWormsWhenRolling(Dice taken);
    ExpectedWorms expectedWormsWhenQuitting(Dice taken);
};

#endif
