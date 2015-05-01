#ifndef GAME_H
#define GAME_H

#include "dice.h"

#include <cassert>
#include <memory>
#include <set>
#include <string>
#include <vector>

extern const int NUM_DICE;

class Game;

class Strategy {
  public:
    Strategy(std::string const name) :
      m_name(name)
    {}
    virtual ~Strategy() {}

    std::string const &name() const {
      return m_name;
    }

    virtual void prepareTurn(Game const &game) = 0;
    virtual bool chooseWhetherToRoll(Game const &game, Dice const &taken) = 0;
    virtual DieSide const *chooseSideToTake(Game const &game, Dice const &taken, Dice const &roll) = 0;

  private:
    Strategy(Strategy const &) = delete;
    Strategy &operator=(Strategy const &) = delete;

    std::string const m_name;
};

typedef int Worms;

class Tile {
  public:
    static std::set<Tile> const ALL;
    static Tile const INVALID;

    Score score() const {
      return m_score;
    }

    Worms worms() const {
      return m_worms;
    }

    bool valid() const {
      return m_worms >= 0;
    }

  private:
    Score m_score;
    Worms m_worms;

    Tile(Score score, Worms worms) :
      m_score(score),
      m_worms(worms)
    {}
};

// Higher tiles are sorted first.
bool operator<(Tile const &a, Tile const &b);
bool operator==(Tile const &a, Tile const &b);
std::ostream &operator<<(std::ostream &out, Tile const &tile);

class Player {
  public:
    Player(std::shared_ptr<Strategy> strategy) :
      m_strategy(strategy)
    {}

    Strategy &strategy() const {
      return *m_strategy;
    }

    std::string const &name() const {
      return m_strategy->name();
    }

    bool hasStrategy(Strategy const *strategy) const {
      return strategy == &*m_strategy;
    }

    bool hasTiles() const {
      return !m_tiles.empty();
    }

    Tile topTile() const {
      return m_tiles.back();
    }

    std::vector<Tile> const &tiles() const {
      return m_tiles;
    }

    void takeTile(Tile tile) {
      m_tiles.push_back(tile);
    }

    Tile popTile() {
      Tile tile = m_tiles.back();
      m_tiles.pop_back();
      return tile;
    }

    Worms worms() const {
      Worms w = 0;
      for (Tile tile : m_tiles) {
        w += tile.worms();
      }
      return w;
    }

  private:
    std::shared_ptr<Strategy> const m_strategy;
    std::vector<Tile> m_tiles;
};

class Game {
  public:
    Game(std::vector<std::shared_ptr<Strategy>> strategies, int startPlayer) :
      m_currentPlayer(startPlayer),
      m_tiles(Tile::ALL)
    {
      assert(!strategies.empty());
      for (std::shared_ptr<Strategy> strategy : strategies) {
        m_players.push_back(Player(strategy));
      }
    }

    int numPlayers() const {
      return m_players.size();
    }

    std::vector<Player> const &players() const {
      return m_players;
    }

    Player const &playerByStrategy(Strategy const *strategy) const {
      for (Player const &player : m_players) {
        if (&player.strategy() == strategy) {
          return player;
        }
      }
      assert(false);
    }

    Player const &currentPlayer() const {
      return m_players[m_currentPlayer];
    }

    std::set<Tile> const &remainingTiles() const {
      return m_tiles;
    }

    Tile stealableTile(Strategy const *thief, Dice const &dice) const;
    Tile bestRemainingTile(Dice const &dice) const;

    void playOneTurn();
    void playToEnd();
    bool isOver() const;
    int winner() const;

  private:
    std::vector<Player> m_players;
    unsigned m_currentPlayer;
    std::set<Tile> m_tiles;

    Score takeTurn(Strategy &strategy) const;
    void takeOrLoseTile(Score score);

    bool canRoll(Dice taken) const {
      return taken.count() < NUM_DICE;
    }

    bool canTakeAny(Dice taken, Dice roll) const {
      for (DieSide const *side : DieSide::ALL) {
        if (roll.contains(side) && !taken.contains(side)) {
          return true;
        }
      }
      return false;
    }

    Player &currentPlayer() {
      return m_players[m_currentPlayer];
    }
};

#endif
