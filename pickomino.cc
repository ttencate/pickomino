#include "bot.h"
#include "game.h"
#include "roll.h"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *const *argv) {
  // for (Roll roll : Roll::allWithDice(2)) {
  //   cout << roll.dice() << ' ' << roll.probability() << '\n';
  // }

  if (argc > 1) {
    seedRandom(atoi(argv[1]));
  }

  vector<shared_ptr<Strategy>> strategies;
  strategies.push_back(shared_ptr<Strategy>(new Bot("Alice")));
  strategies.push_back(shared_ptr<Strategy>(new Bot("Bob")));
  Game game(strategies);
  game.playToEnd();
}
