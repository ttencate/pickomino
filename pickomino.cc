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

typedef char DieSide;
const DieSide ONE = 0;
const DieSide TWO = 1;
const DieSide THREE = 2;
const DieSide FOUR = 3;
const DieSide FIVE = 4;
const DieSide WORM = 5;
const DieSide NUM_DIE_SIDES = 6;

typedef int Score;
typedef double Probability;
typedef double ExpectedWorms;

int scoreForDie(DieSide v) {
  if (v == WORM) {
    return 5;
  } else {
    return v + 1;
  }
}

char const *stringForDie(DieSide v) {
  switch (v) {
    case ONE: return "1";
    case TWO: return "2";
    case THREE: return "3";
    case FOUR: return "4";
    case FIVE: return "5";
    case WORM: return "w";
    default: return "ERROR";
  }
}

int nChooseK(int n, int k) {
  int out = 1;
  for (int i = 1; i < k; ++i) {
    out *= (n + 1 - i) / i;
  }
  return out;
}


class Dice {
  public:
    Dice() :
      d{0}
    {
    }

    int operator[](DieSide s) const {
      return d[(int) s];
    }

    int &operator[](DieSide s) {
      return d[(int) s];
    }

    bool contains(DieSide s) const {
      return d[(int) s] > 0;
    }

    int numDice() const {
      int n = 0;
      for (DieSide i = 0; i < NUM_DIE_SIDES; ++i) {
        n += d[(int) i];
      }
      return n;
    }

    Score sum() const {
      int s = 0;
      for (DieSide i = 0; i < NUM_DIE_SIDES; ++i) {
        s += d[(int) i] * scoreForDie(i);
      }
      return s;
    }

  private:
    int d[NUM_DIE_SIDES];
};

ostream &operator<<(ostream &out, Dice const &dice) {
  for (DieSide i = 0; i < NUM_DIE_SIDES; ++i) {
    for (int j = 0; j < dice[i]; ++j) {
      out << stringForDie(i);
    }
  }
  return out;
}

istream &operator>>(istream &in, Dice &dice) {
  string input;
  if (in >> input) {
    for (unsigned i = 0; i < input.size(); ++i) {
      char c = input[i];
      if (c >= '1' && c <= '5') {
        dice[c - '1']++;
      } else if (c == 'w' || c == 'W') {
        dice[WORM]++;
      }
    }
  }
  return in;
}

long int factorial(int n) {
  long int f = 1;
  for (; n; --n) {
    f *= n;
  }
  return f;
}

long int power(int n, int k) {
  long int p = 1;
  for (; k; --k) {
    p *= n;
  }
  return p;
}

Probability rollProbability(Dice const &dice) {
  int n = dice.numDice();
  long int numerator = factorial(n);
  long int denominator = power(NUM_DIE_SIDES, n);
  for (DieSide side = 0; side < NUM_DIE_SIDES; ++side) {
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

void enumerateRolls(vector<Roll> &out, int numDice, Dice dice = Dice(), int usedDice = 0, DieSide dieSide = 0) {
  int remainingDice = numDice - usedDice;
  if (dieSide == NUM_DIE_SIDES) {
    out.push_back(Roll(dice));
    return;
  }
  int minDiceToUse = dieSide == NUM_DIE_SIDES - 1 ? remainingDice : 0;
  for (int num = remainingDice; num >= minDiceToUse; --num) {
    Dice newRoll = dice;
    newRoll[dieSide] = num;
    enumerateRolls(out, numDice, newRoll, usedDice + num, dieSide + 1);
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

ExpectedWorms expectedWormsWhenTaking(GameState const &state, Dice taken, Dice const &roll, DieSide side);

ExpectedWorms expectedWormsWhenRolled(GameState const &state, Dice taken, Dice const &roll) {
  ExpectedWorms w = state.wormsForDeath();
  for (DieSide side = 0; side < NUM_DIE_SIDES; ++side) {
    if (roll.contains(side) && !taken.contains(side)) {
      w = max(w, expectedWormsWhenTaking(state, taken, roll, side));
    }
  }
  return w;
}

ExpectedWorms expectedWormsWhenRolling(GameState const &state, Dice taken) {
  int remainingDice = NUM_DICE - taken.numDice();
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

ExpectedWorms expectedWormsWhenTaking(GameState const &state, Dice taken, Dice const &roll, DieSide side) {
  assert(roll.contains(side));
  assert(!taken.contains(side));

  taken[side] = roll[side];

  ExpectedWorms w = state.wormsForDeath();
  if (taken.contains(WORM)) {
    w = max(w, expectedWormsWhenQuitting(state, taken));
  }
  if (taken.numDice() < NUM_DICE) {
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
  if (roll.numDice() > 0 && roll.numDice() + taken.numDice() != NUM_DICE) {
    cerr << "Must be " << NUM_DICE << " dice in total\n";
    return 1;
  }

  cout << '\n';

  for (int i = 0; i <= NUM_DICE; ++i) {
    enumerateRolls(rolls[i], i);
  }

  if (roll.numDice() > 0) {
    DieSide best = -1;
    ExpectedWorms wBest = -100;
    for (DieSide i = 0; i < NUM_DIE_SIDES; ++i) {
      if (roll.contains(i) && !taken.contains(i)) {
        ExpectedWorms w = expectedWormsWhenTaking(state, taken, roll, i);
        if (w > wBest) {
          wBest = w;
          best = i;
        }
        cout << "When taking " << stringForDie(i) << ", expect " << w << " worms\n";
      }
    }
    if (best < 0) {
      cout << "You're already dead\n";
    } else {
      cout << "Take " << stringForDie(best) << ", then ";
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
