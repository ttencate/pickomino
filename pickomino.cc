#include "bot.h"
#include "dice.h"
#include "game.h"
#include "maths.h"
#include "roll.h"

#include <iostream>
#include <vector>

using namespace std;

int main() {
  // for (Roll roll : Roll::allWithDice(2)) {
  //   cout << roll.dice() << ' ' << roll.probability() << '\n';
  // }

  vector<shared_ptr<Strategy>> strategies;
  strategies.push_back(shared_ptr<Strategy>(new Bot()));
  Game game(strategies);
  game.play();
}
