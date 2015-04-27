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
  Bot bot(&game);
  Dice taken;

  while (game.canRoll(taken)) {
    bot.prepareTurn();
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
      return 1;
    }

    // TODO this crashes when none can be taken

    DieSide const *side = bot.chooseSideToTake(taken, roll);
    cout << "Take " << side->toString() << '\n';
    taken[side] = roll[side];
  }
}
