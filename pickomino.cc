#include "bot.h"
#include "dice.h"
#include "game.h"
#include "maths.h"
#include "roll.h"

#include <iostream>

using namespace std;

int main() {
  // for (Roll roll : Roll::allWithDice(2)) {
  //   cout << roll.dice() << ' ' << roll.probability() << '\n';
  // }

  int wormsToLose;
  cout << "Number of worms on top of stack: ";
  if (!(cin >> wormsToLose)) {
    cerr << "Must be a number\n";
    return 1;
  }

  Game game(wormsToLose);

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

  Bot bot(&game);
  bot.prepareTurn();

  DieSide const *side = bot.chooseSideToTake(taken, roll);
  cout << "Take " << side->toString() << '\n';

  taken[side] = roll[side];
  if (bot.chooseWhetherToRoll(taken)) {
    cout << "Roll again\n";
  } else {
    cout << "Quit\n";
  }
}
