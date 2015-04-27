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

  int lowestTile;
  cout << "Lowest tile to be claimed: ";
  if (!(cin >> lowestTile)) {
    cerr << "Must be a number\n";
    return 1;
  }

  Game game(wormsToLose, lowestTile);
  Bot bot(&game);
  Dice taken;

  bot.prepareTurn();

  while (true) {
    if (!game.canRoll(taken)) {
      cout << "Out of dice\n";
      break;
    }

    cout << '\n';

    if (!bot.chooseWhetherToRoll(taken)) {
      cout << "Quit\n";
      break;
    }

    Dice roll;
    while (true) {
      cout << "Roll: ";
      cin >> roll;
      if (roll.count() == NUM_DICE - taken.count()) {
        break;
      }
      cerr << "Must be " << NUM_DICE - taken.count() << " dice\n";
    }

    if (!game.canTakeAny(taken, roll)) {
      cout << "Died\n";
      return 0;
    }

    DieSide const *side = bot.chooseSideToTake(taken, roll);
    cout << "Take " << side->toString() << '\n';
    taken[side] = roll[side];
  }
}
