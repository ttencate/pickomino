#include "game.h"
#include "roll.h"

#include <iostream>

using namespace std;

const int NUM_DICE = 8;

Score Player::takeTurn(Game const &game) {
  m_strategy->prepareTurn(game);
  Dice taken;
  while (game.canRoll(taken) && m_strategy->chooseWhetherToRoll(game, taken)) {
    Dice roll = randomRoll(NUM_DICE - taken.count());
    cout << "Rolled " << roll << '\n';

    if (!game.canTakeAny(taken, roll)) {
      cout << "Died\n";
      return 0;
    }

    DieSide const *side = m_strategy->chooseSideToTake(game, taken, roll);
    taken[side] = roll[side];
    cout << "Took " << side->toString() << ", now have " << taken << '\n';
  }
  cout << "Quit\n";
  return taken.sum();
}

void Game::play() {
  int currentPlayer = 0;
  Score score = m_players[currentPlayer].takeTurn(*this);
  cout << "Scored " << score << '\n';
}
