#ifndef ROLL_H
#define ROLL_H

#include "dice.h"

typedef double Probability;

class Roll {
  public:
    Roll(Dice const &dice);

    Dice const &dice() const {
      return m_dice;
    }

    Probability probability() const {
      return m_probability;
    }

    static std::vector<Roll> const &allWithDice(unsigned numDice);

  private:
    Dice const m_dice;
    Probability const m_probability;
};

void seedRandom(int seed);
Dice randomRoll(int n);

#endif
