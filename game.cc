#include "game.h"
#include "roll.h"

#include <iostream>

using namespace std;

const int NUM_DICE = 8;

set<Tile> const Tile::ALL = {
  Tile(21, 1),
  Tile(22, 1),
  Tile(23, 1),
  Tile(24, 1),
  
  Tile(25, 2),
  Tile(26, 2),
  Tile(27, 2),
  Tile(28, 2),

  Tile(29, 3),
  Tile(30, 3),
  Tile(31, 3),
  Tile(32, 3),

  Tile(33, 4),
  Tile(34, 4),
  Tile(35, 4),
  Tile(36, 4),
};

Tile const Tile::INVALID = Tile(-1, -1);

// Higher tiles are sorted first.
bool operator<(Tile const &a, Tile const &b) {
  if (a.worms() < b.worms()) return false;
  if (a.worms() > b.worms()) return true;
  if (a.score() < b.score()) return false;
  if (a.score() > b.score()) return true;
  return true;
}

bool operator==(Tile const &a, Tile const &b) {
  return a.worms() == b.worms() && a.score() == b.score();
}

std::ostream &operator<<(std::ostream &out, Tile const &tile) {
  out << tile.score() << '|';
  for (int i = 0; i < tile.worms(); i++) {
    out << 'W';
  }
  return out;
}

Tile Game::stealableTile(Strategy const *thief, Dice const &dice) const {
  if (!dice.contains(DieSide::WORM)) {
    return Tile::INVALID;
  }
  Score score = dice.sum();
  for (Player const &player : m_players) {
    if (!player.hasStrategy(thief) && player.hasTiles() && player.topTile().score() == score) {
      return player.topTile();
    }
  }
  return Tile::INVALID;
}

Tile Game::bestRemainingTile(Dice const &dice) const {
  if (!dice.contains(DieSide::WORM)) {
    return Tile::INVALID;
  }
  Score score = dice.sum();
  for (Tile tile : m_tiles) {
    if (score >= tile.score()) {
      return tile;
    }
  }
  return Tile::INVALID;
}

void Game::playOneTurn() {
  cout << "Remaining tiles:";
  for (auto i = m_tiles.rbegin(); i != m_tiles.rend(); i++) {
    cout << ' ' << *i;
  }
  cout << '\n';
  cout << '\n';

  cout << currentPlayer().name() << "'s turn\n";
  Dice taken = takeTurn(currentPlayer().strategy());
  cout << currentPlayer().name() << " scored " << taken.sum() << '\n';
  takeOrLoseTile(taken);

  m_currentPlayer = (m_currentPlayer + 1) % m_players.size();
}

void Game::playToEnd() {
  while (!isOver()) {
    playOneTurn();
  }
  cout << '\n';
  cout << "Game over!\n";
  for (Player const &player : m_players) {
    cout << player.name() << '\t' << player.worms() << '\t';
    for (Tile tile : player.tiles()) {
      cout << tile << ' ';
    }
    cout << '\n';
  }
}

bool Game::isOver() const {
  return remainingTiles().empty();
}

int Game::winner() const {
  Worms maxWorms = -1;
  int winner = -1;
  for (unsigned i = 0; i < m_players.size(); ++i) {
    if (m_players[i].worms() > maxWorms) {
      maxWorms = m_players[i].worms();
      winner = i;
    } else if (m_players[i].worms() == maxWorms) {
      return -1; // Tie.
    }
  }
  return winner;
}

Dice Game::takeTurn(Strategy &strategy) const {
  strategy.prepareTurn(*this);
  Dice taken;
  while (canRoll(taken) && strategy.chooseWhetherToRoll(*this, taken)) {
    Dice roll = randomRoll(NUM_DICE - taken.count());
    cout << currentPlayer().name() << " rolled " << roll << '\n';

    if (!canTakeAny(taken, roll)) {
      cout << currentPlayer().name() << " died\n";
      return Dice();
    }

    DieSide const *side = strategy.chooseSideToTake(*this, taken, roll);
    taken[side] = roll[side];
    cout << currentPlayer().name() << " took " << side->toString() << ", now has " << taken << '\n';
  }
  cout << "Quit\n";
  return taken;
}

void Game::takeOrLoseTile(Dice taken) {
  if (taken.contains(DieSide::WORM)) {
    Score score = taken.sum();

    for (unsigned i = 0; i < m_players.size(); i++) {
      Player &victim = m_players[i];
      if (i != m_currentPlayer && victim.hasTiles() && victim.topTile().score() == score) {
        Tile stolen = victim.popTile();
        currentPlayer().takeTile(stolen);
        cout << currentPlayer().name() << " stole " << stolen << " from " << victim.name() << '\n';
        return;
      }
    }

    for (auto i = m_tiles.begin(); i != m_tiles.end(); i++) {
      if (score >= i->score()) {
        Tile taken = *i;
        currentPlayer().takeTile(taken);
        m_tiles.erase(i);
        cout << currentPlayer().name() << " took " << taken << " from the table\n";
        return;
      }
    }
  }

  Tile returned = Tile::INVALID;
  if (currentPlayer().hasTiles()) {
    returned = currentPlayer().popTile();
    m_tiles.insert(returned);  
    cout << currentPlayer().name() << " returned " << returned << " to the table\n";
  } else {
    cout << currentPlayer().name() << " has nothing to hand in\n";
  }

  assert(!m_tiles.empty());
  if (*m_tiles.begin() == returned) {
    cout << "Returned tile is highest on the table, not turning any tiles over\n";
  } else {
    cout << "Turning over highest tile, " << *m_tiles.begin() << '\n';
    m_tiles.erase(m_tiles.begin());
  }
}
