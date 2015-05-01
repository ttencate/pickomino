#include "maths.h"
#include "roll.h"

#include <random>

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

static std::mt19937 rnd(4); // Chosen by fair dice roll. Guaranteed to be random.
static std::uniform_int_distribution<int> distribution(0, DieSide::COUNT - 1);

void seedRandom(int seed) {
  rnd.seed(seed);
}

DieSide const *randomDieSide() {
  return DieSide::ALL[distribution(rnd)];
}

Dice randomRoll(int n) {
  Dice dice;
  for (int i = 0; i < n; i++) {
    dice[randomDieSide()]++;
  }
  return dice;
}
