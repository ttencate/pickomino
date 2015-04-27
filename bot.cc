#include "bot.h"
#include "roll.h"

#include <algorithm>
#include <cassert>
#include <iostream>

using namespace std;

void Bot::prepareTurn() {
  m_expectedWhenRolling.clear();
  ExpectedWorms w = expectedWormsWhenRolling(Dice());
  cout << "This turn, expect " << w << " worms\n";
  if (w < 0) {
    cout << "I would rather not play...\n";
  }
}

bool Bot::chooseWhetherToRoll(Dice const &taken) {
  assert(m_game->canRoll(taken));
  ExpectedWorms whenRolling = expectedWormsWhenRolling(taken);
  ExpectedWorms whenQuitting = expectedWormsWhenQuitting(taken);
  cout << "When rolling, expect " << whenRolling << " worms\n";
  cout << "When quitting, expect " << whenQuitting << " worms\n";
  return whenRolling > whenQuitting;
}

DieSide const *Bot::chooseSideToTake(Dice const &taken, Dice const &roll) {
  DieSide const *best = nullptr;
  ExpectedWorms bestW = -100;
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      ExpectedWorms w = expectedWormsWhenTaking(taken, roll, side);
      if (w > bestW) {
        bestW = w;
        best = side;
      }
      cout << "When taking " << side->toString() << ", expect " << w << " worms\n";
    }
  }
  assert(best);
  if (bestW < 0) {
    cout << "I'm getting nervous...\n";
  } else if (bestW > 2) {
    cout << "Way to go!\n";
  }
  return best;
}

ExpectedWorms Bot::expectedWormsWhenRolled(Dice taken, Dice const &roll) {
  ExpectedWorms w = m_game->wormsForDeath();
  for (DieSide const *side : DieSide::ALL) {
    if (roll.contains(side) && !taken.contains(side)) {
      w = max(w, expectedWormsWhenTaking(taken, roll, side));
    }
  }
  return w;
}

ExpectedWorms Bot::expectedWormsWhenRolling(Dice taken) {
  if (m_expectedWhenRolling.find(taken) == m_expectedWhenRolling.end()) {
    int remainingDice = NUM_DICE - taken.count();
    assert(remainingDice > 0);

    ExpectedWorms w = 0;
    for (Roll roll : Roll::allWithDice(remainingDice)) {
      w += roll.probability() * expectedWormsWhenRolled(taken, roll.dice());
    }

    m_expectedWhenRolling[taken] = w;
  }
  return m_expectedWhenRolling[taken];
}

ExpectedWorms Bot::expectedWormsWhenQuitting(Dice taken) {
  return m_game->wormsForScore(taken.sum());
}

ExpectedWorms Bot::expectedWormsWhenTaking(Dice taken, Dice const &roll, DieSide const *side) {
  assert(roll.contains(side));
  assert(!taken.contains(side));

  taken[side] = roll[side];

  ExpectedWorms w = expectedWormsWhenQuitting(taken);
  if (m_game->canRoll(taken)) {
    w = max(w, expectedWormsWhenRolling(taken));
  }
  return w;
}
