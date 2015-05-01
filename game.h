#ifndef GAME_H
#define GAME_H

#include "dice.h"

#include <memory>
#include <vector>

extern const int NUM_DICE;

class Game;

class Strategy {
  public:
    Strategy() {}
    virtual ~Strategy() {}

    virtual void prepareTurn(Game const &game) = 0;
    virtual bool chooseWhetherToRoll(Game const &game, Dice const &taken) = 0;
    virtual DieSide const *chooseSideToTake(Game const &game, Dice const &taken, Dice const &roll) = 0;

  private:
    Strategy(Strategy const &) = delete;
    Strategy &operator=(Strategy const &) = delete;
};

class Player {
  public:
    Player(std::shared_ptr<Strategy> strategy) :
      m_strategy(strategy)
    {}

    Score takeTurn(Game const &game);

  private:
    std::shared_ptr<Strategy> const m_strategy;
};

typedef int Worms;

class Tile {
  public:
    Tile(Score score, Worms worms) :
      m_score(score),
      m_worms(worms)
    {}

    Score score() const {
      return m_score;
    }

    Worms worms() const {
      return m_worms;
    }

  private:
    Score const m_score;
    Worms const m_worms;
};

class Game {
  public:
    Game(std::vector<std::shared_ptr<Strategy>> strategies) {
      for (std::shared_ptr<Strategy> strategy : strategies) {
        m_players.push_back(Player(strategy));
      }
    }

    Worms wormsForScore(Score score) const {
      // TODO
      return score;
    }

    int wormsForDeath() const {
      // TODO
      return 0;
    }

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

    void play();

  private:
    std::vector<Player> m_players;
};

#endif
