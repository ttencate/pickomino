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

  Game game;
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

    Dice roll = randomRoll(NUM_DICE - taken.count());
    cout << "Rolled " << roll << '\n';

    if (!game.canTakeAny(taken, roll)) {
      cout << "Died\n";
      return 0;
    }

    DieSide const *side = bot.chooseSideToTake(taken, roll);
    taken[side] = roll[side];
    cout << "Took " << side->toString() << ", now have " << taken << '\n';
  }
}
