#include <array>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

const int NUM_DICE = 8;
const int NUM_DIE_SIDES = 6;
const int MAX_DIE_VALUE = 5;

typedef char DieSide;
const DieSide ONE = 0;
const DieSide TWO = 1;
const DieSide THREE = 2;
const DieSide FOUR = 3;
const DieSide FIVE = 4;
const DieSide WORM = 5;

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


class Roll {
  public:
    Roll() :
      n(0),
      pNumerator(1),
      pDenominator(1)
    {
      for (int i = 0; i < NUM_DIE_SIDES; ++i) {
        d[i] = 0;
      }
    }

    int operator[](DieSide s) const {
      return d[(int) s];
    }

    // Assumes that (*this)[s] == 0.
    void addDice(DieSide s, int num) {
      // Multinomial gives number of ways to roll this:
      //   (n choose k1,...,k6) = n! / (k1!...k6)
      // Divide by 6^n to find the probability of this particular roll:
      //   n! / (k1!...k6!6^n)
      d[(int) s] = num;
      for (int i = 1; i <= num; ++i) {
        ++n;
        pNumerator *= n;
        pDenominator *= NUM_DIE_SIDES;
        pDenominator *= i;
      }
    }

    double probability() const {
      return (double) pNumerator / pDenominator;
    }

  private:
    int d[NUM_DIE_SIDES];

    int n;
    long int pNumerator;
    long int pDenominator;
};

ostream &operator<<(ostream &out, Roll const &roll) {
  for (int i = 0; i < NUM_DIE_SIDES; ++i) {
    for (int j = 0; j < roll[i]; ++j) {
      out << stringForDie(i);
    }
  }
  return out;
}

void enumerateRolls(vector<Roll> &out, int remainingDice, Roll partialRoll = Roll(), DieSide dieSide = 0) {
  if (dieSide == NUM_DIE_SIDES - 1) {
    partialRoll.addDice(dieSide, remainingDice);
    out.push_back(partialRoll);
    return;
  }
  for (int i = remainingDice; i >= 0; --i) {
    Roll roll = partialRoll;
    roll.addDice(dieSide, i);
    enumerateRolls(out, remainingDice - i, roll, dieSide + 1);
  }
}

int main() {
  int wormsToLose;
  cout << "Number of worms on top of stack: ";
  cin >> wormsToLose;
  cout << '\n';

  vector<int> wormsForScore(NUM_DICE * MAX_DIE_VALUE + 1);
  for (unsigned i = 0; i < wormsForScore.size(); ++i) {
    if (i < 21) {
      wormsForScore[i] = 0;
    } else if (i < 25) {
      wormsForScore[i] = 1;
    } else if (i < 29) {
      wormsForScore[i] = 2;
    } else if (i < 33) {
      wormsForScore[i] = 3;
    } else {
      wormsForScore[i] = 4;
    }
  }

  vector<vector<Roll>> possibleRolls(NUM_DICE + 1);
  for (int i = 0; i <= NUM_DICE; ++i) {
    enumerateRolls(possibleRolls[i], i);
  }
}
