#include "dice.h"
#include "maths.h"

#include <algorithm>
#include <array>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
#undef max

const int NUM_DICE = 8;
const int MAX_DIE_VALUE = 5;

typedef double Probability;
typedef double ExpectedWorms;

Probability rollProbability(Dice const &dice) {
  int n = dice.count();
  long int numerator = factorial(n);
  long int denominator = power(DieSide::COUNT, n);
  for (DieSide const *side : DieSide::ALL) {
    denominator *= factorial(dice[side]);
  }
  return (Probability) numerator / denominator;
}

class Roll {
  public:
    Roll(Dice const &dice) :
      m_dice(dice),
      m_probability(rollProbability(dice))
    {
    }

    Dice const &dice() const {
      return m_dice;
    }

    Probability probability() const {
      return m_probability;
    }

  private:
    Dice const m_dice;
    Probability const m_probability;
};

void enumerateRolls(vector<vector<Roll>> &out, int remainingDice, Dice dice = Dice(), vector<DieSide const *>::const_iterator side = DieSide::ALL.begin()) {
  if (side == DieSide::ALL.end()) {
    unsigned n = dice.count();
    if (out.size() <= n) {
      out.resize(n + 1);
    }
    out[n].push_back(Roll(dice));
    return;
  }
  for (int num = remainingDice; num >= 0; --num) {
    Dice newRoll = dice;
    newRoll[*side] = num;
    enumerateRolls(out, remainingDice, newRoll, side + 1);
  }
}

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

// TODO make not global
vector<vector<Roll>> rolls(NUM_DICE + 1);

ExpectedWorms expectedWormsWhenTaking(GameState const &state, Dice taken, Dice const &roll, DieSide const *side);

ExpectedWorms expectedWormsWhenRolled(GameState const &state, Dice taken, Dice const &roll) {
  ExpectedWorms w = state.wormsForDeath();
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      w = max(w, expectedWormsWhenTaking(state, taken, roll, side));
    }
  }
  return w;
}

ExpectedWorms expectedWormsWhenRolling(GameState const &state, Dice taken) {
  int remainingDice = NUM_DICE - taken.count();
  assert(remainingDice > 0);

  ExpectedWorms w = 0;
  for (unsigned i = 0; i < rolls[remainingDice].size(); ++i) {
    Roll roll = rolls[remainingDice][i];
    w += roll.probability() * expectedWormsWhenRolled(state, taken, roll.dice());
  }
  return w;
}

ExpectedWorms expectedWormsWhenQuitting(GameState const &state, Dice taken) {
  return state.wormsForScore(taken.sum());
}

ExpectedWorms expectedWormsWhenTaking(GameState const &state, Dice taken, Dice const &roll, DieSide const *side) {
  assert(roll.contains(side));
  assert(!taken.contains(side));

  taken[side] = roll[side];

  ExpectedWorms w = state.wormsForDeath();
  if (taken.contains(DieSide::WORM)) {
    w = max(w, expectedWormsWhenQuitting(state, taken));
  }
  if (taken.count() < NUM_DICE) {
    w = max(w, expectedWormsWhenRolling(state, taken));
  }
  return w;
}

int main() {
  int wormsToLose;
  cout << "Number of worms on top of stack: ";
  if (!(cin >> wormsToLose)) {
    cerr << "Must be a number\n";
    return 1;
  }

  GameState state(wormsToLose);

  Dice taken;
  cout << "Taken: ";
  if (!(cin >> taken)) {
    cerr << "Must be a string of dice\n";
    return 1;
  }

  Dice roll;
  cout << "Roll: ";
  if (!(cin >> roll)) {
    cerr << "Must be a string of dice\n";
    return 1;
  }
  if (roll.count() > 0 && roll.count() + taken.count() != NUM_DICE) {
    cerr << "Must be " << NUM_DICE << " dice in total\n";
    return 1;
  }

  cout << '\n';

  enumerateRolls(rolls, NUM_DICE);

  if (roll.count() > 0) {
    DieSide const *best = nullptr;
    ExpectedWorms wBest = -100;
    for (DieSide const *side : DieSide::ALL) {
      if (roll.contains(side) && !taken.contains(side)) {
        ExpectedWorms w = expectedWormsWhenTaking(state, taken, roll, side);
        if (w > wBest) {
          wBest = w;
          best = side;
        }
        cout << "When taking " << side->toString() << ", expect " << w << " worms\n";
      }
    }
    if (!best) {
      cout << "You're already dead\n";
    } else {
      cout << "Take " << best->toString() << ", then ";
      taken[best] = roll[best];

      ExpectedWorms whenRolling = expectedWormsWhenRolling(state, taken);
      ExpectedWorms whenQuitting = expectedWormsWhenQuitting(state, taken);
      if (whenRolling > whenQuitting) {
        cout << "roll again\n";
      } else {
        cout << "quit\n";
      }
    }
  }

  // To print probabilities for dice rolls:
  //
  // for (unsigned j = 0; j <= 3; ++j) {
  //   cout << "With " << j << " dice:\n";
  //   for (unsigned i = 0; i < rolls[j].size(); ++i) {
  //     cout << rolls[j][i].roll() << ' ' << rolls[j][i].probability() << '\n';
  //   }
  // }
}
