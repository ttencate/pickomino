#ifndef ROLL_H
#define ROLL_H

#include "dice.h"

typedef double Probability;

extern const int NUM_DICE;

class Roll {
  public:
    Roll(Dice const &dice);

    Dice const &dice() const {
      return m_dice;
    }

    Probability probability() const {
      return m_probability;
    }

    static std::vector<Roll> const &allWithDice(unsigned numDice) {
      return s_all[numDice];
    }

  private:
    Dice const m_dice;
    Probability const m_probability;

    static std::vector<std::vector<Roll>> const s_all;
};

#endif
