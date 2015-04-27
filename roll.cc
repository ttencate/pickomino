#include "maths.h"
#include "roll.h"

using namespace std;

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

void enumerateRolls(vector<Roll> &out, int remainingDice, Dice dice, vector<DieSide const *>::const_iterator side) {
  if (side == DieSide::ALL.end() - 1) {
    dice[*side] = remainingDice;
    out.push_back(Roll(dice));
    return;
  }
  for (int num = remainingDice; num >= 0; --num) {
    Dice newRoll = dice;
    newRoll[*side] = num;
    enumerateRolls(out, remainingDice - num, newRoll, side + 1);
  }
}

vector<Roll> const &Roll::allWithDice(unsigned numDice) {
  static vector<vector<Roll>*> cache;
  if (cache.size() <= numDice) {
    cache.resize(numDice + 1);
  }
  if (!cache[numDice]) {
    cache[numDice] = new vector<Roll>();
    enumerateRolls(*cache[numDice], numDice, Dice(), DieSide::ALL.begin());
  }
  return *cache[numDice];
}
