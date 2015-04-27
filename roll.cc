#include "maths.h"
#include "roll.h"

using namespace std;

const int NUM_DICE = 8;

Probability rollProbability(Dice const &dice) {
  int n = dice.count();
  long int numerator = factorial(n);
  long int denominator = power(DieSide::COUNT, n);
  for (DieSide const *side : DieSide::ALL) {
    denominator *= factorial(dice[side]);
  }
  return (Probability) numerator / denominator;
}

Roll::Roll(Dice const &dice) :
  m_dice(dice),
  m_probability(rollProbability(dice))
{
}

void enumerateRolls(vector<vector<Roll>> &out, int remainingDice, Dice dice, vector<DieSide const *>::const_iterator side) {
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

vector<vector<Roll>> enumerateRolls(unsigned numDice) {
  vector<vector<Roll>> rolls;
  enumerateRolls(rolls, numDice, Dice(), DieSide::ALL.begin());
  return rolls;
}

std::vector<std::vector<Roll>> const Roll::s_all = enumerateRolls(NUM_DICE);
