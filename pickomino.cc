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

  int n_players = 2;
  if (argc > 2) {
    n_players = atoi(argv[2]);
  }

  if (argc > 3) {
    seedRandom(atoi(argv[3]));
  }

  int optimalWins = 0;
  int simpleWins = 0;
  int ties = 0;
  while (games--) {
    vector<shared_ptr<Strategy>> strategies;
    strategies.push_back(shared_ptr<Strategy>(new OptimalBot("Alice")));
    strategies.push_back(shared_ptr<Strategy>(new SimpleBot("Bob")));
    if (n_players > 2) {
	    for (int i=2; i< n_players; i++) {
    		strategies.push_back(shared_ptr<Strategy>(new OptimalBot("Optimal." + std::to_string(i))));
	    }
    }
    Game game(strategies, games % 2);
    game.playToEnd();
    switch (game.winner()) {
      case 0:
        optimalWins++;
        break;
      case 1:
        simpleWins++;
        break;
      case -1:
        ties++;
        break;
      default:
        optimalWins++;
        break;
    }
  }
  cout << '\n';
  cout << "Optimal Strategy wins " << optimalWins << " times\n";
  cout << "Simple strategy wins " << simpleWins << " times\n";
  if (ties) {
    cout << "There were " << ties << " ties\n";
  }
}
