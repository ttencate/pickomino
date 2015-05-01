#include "optimal_bot.h"
#include "roll.h"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>

using namespace std;

void OptimalBot::prepareTurn(Game const &game) {
  Player const &me = game.playerByStrategy(this);
  m_expectedWhenDead = me.hasTiles() ? -me.topTile().worms() : 0;
  m_expectedWhenRolling.clear();
  m_expectedWhenQuitting.clear();
}

bool OptimalBot::chooseWhetherToRoll(Game const &game, Dice const &taken) {
  ExpectedWorms whenRolling = expectedWormsWhenRolling(game, taken);
  ExpectedWorms whenQuitting = expectedWormsWhenQuitting(game, taken);
  cout << "\"When rolling, I expect " << whenRolling << " worms.\"\n";
  cout << "\"When quitting, I expect " << whenQuitting << " worms.\"\n";
  return whenRolling > whenQuitting;
}

DieSide const *OptimalBot::chooseSideToTake(Game const &game, Dice const &taken, Dice const &roll) {
  DieSide const *best = nullptr;
  ExpectedWorms bestW = -numeric_limits<ExpectedWorms>::max();
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      ExpectedWorms w = expectedWormsWhenTaking(game, taken, roll, side);
      if (w > bestW) {
        bestW = w;
        best = side;
      }
      cout << "\"When taking " << side->toString() << ", I expect " << w << " worms.\"\n";
    }
  }
  assert(best);
  return best;
}

ExpectedWorms OptimalBot::expectedWormsWhenRolled(Game const &game, Dice taken, Dice const &roll) {
  ExpectedWorms w = m_expectedWhenDead;
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      w = max(w, expectedWormsWhenTaking(game, taken, roll, side));
    }
  }
  return w;
}

ExpectedWorms OptimalBot::expectedWormsWhenRolling(Game const &game, Dice taken) {
  if (m_expectedWhenRolling.find(taken) == m_expectedWhenRolling.end()) {
    int remainingDice = NUM_DICE - taken.count();
    assert(remainingDice > 0);

    ExpectedWorms w = 0;
    for (Roll roll : Roll::allWithDice(remainingDice)) {
      w += roll.probability() * expectedWormsWhenRolled(game, taken, roll.dice());
    }

    m_expectedWhenRolling[taken] = w;
  }
  return m_expectedWhenRolling[taken];
}

ExpectedWorms OptimalBot::expectedWormsWhenQuitting(Game const &game, Dice taken) {
  Score score = taken.sum();
  if (m_expectedWhenQuitting.find(score) == m_expectedWhenQuitting.end()) {
    ExpectedWorms w = m_expectedWhenDead;

    Tile stealable = game.stealableTile(this, score);
    if (stealable.valid()) {
      w = max(w, stealable.worms() * (1 + 1.0 / (game.numPlayers() - 1)));
    }

    Tile remaining = game.bestRemainingTile(score);
    if (remaining.valid()) {
      w = max(w, ExpectedWorms(remaining.worms()));
    }

    m_expectedWhenQuitting[score] = w;
  }
  return m_expectedWhenQuitting[score];
}

ExpectedWorms OptimalBot::expectedWormsWhenTaking(Game const &game, Dice taken, Dice const &roll, DieSide const *side) {
  assert(roll.contains(side));
  assert(!taken.contains(side));

  taken[side] = roll[side];

  ExpectedWorms w = expectedWormsWhenQuitting(game, taken);
  if (taken.count() < NUM_DICE) {
    w = max(w, expectedWormsWhenRolling(game, taken));
  }
  return w;
}
