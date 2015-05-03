#include "optimal_bot.h"
#include "game.h"
#include "human.h"
#include "roll.h"
#include "simple_bot.h"

#include <cstdlib>
#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *const *argv) {
  // for (Roll roll : Roll::allWithDice(2)) {
  //   cout << roll.dice() << ' ' << roll.probability() << '\n';
  // }
  // for (int i = 1; i <= 8; i++) {
  //   cout << i << ' ' << Roll::allWithDice(i).size() << '\n';
  // }
  // return 0;

  int games = 1;
  if (argc > 1) {
    games = atoi(argv[1]);
  }
  if (argc > 2) {
    seedRandom(atoi(argv[2]));
  }

  int optimalWins = 0;
  int simpleWins = 0;
  int ties = 0;
  while (games--) {
    vector<shared_ptr<Strategy>> strategies;
    strategies.push_back(shared_ptr<Strategy>(new OptimalBot("Alice")));
    strategies.push_back(shared_ptr<Strategy>(new SimpleBot("Bob")));
    Game game(strategies, games % 2);
    game.playToEnd();
    switch (game.winner()) {
      case 0:
        optimalWins++;
        break;
      case 1:
        simpleWins++;
        break;
      default:
        ties++;
        break;
    }
  }
  cout << '\n';
  cout << "Alice wins " << optimalWins << " times\n";
  cout << "Bob wins " << simpleWins << " times\n";
  if (ties) {
    cout << "There were " << ties << " ties\n";
  }
}
