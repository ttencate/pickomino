#ifndef DICE_H
#define DICE_H

#include <istream>
#include <ostream>
#include <vector>

typedef int Score;

class DieSide {
  public:
    static std::vector<DieSide const *> const ALL;
    static DieSide const *const WORM;
    static unsigned const COUNT = 6;

    unsigned index() const {
      return m_index;
    }

    Score score() const {
      return m_score;
    }

    char const *toString() const {
      return m_string;
    }

    char inputChar() const {
      return m_inputChar;
    }

  private:
    DieSide(DieSide const &) = delete;
    DieSide &operator=(DieSide const &) = delete;

    DieSide(unsigned index, Score score, char const *string, char inputChar) :
      m_index(index),
      m_score(score),
      m_string(string),
      m_inputChar(inputChar)
    {}

    const unsigned m_index;
    const Score m_score;
    char const *const m_string;
    char const m_inputChar;

    static DieSide const S_ONE;
    static DieSide const S_TWO;
    static DieSide const S_THREE;
    static DieSide const S_FOUR;
    static DieSide const S_FIVE;
    static DieSide const S_WORM;
};

class Dice {
  public:
    Dice() :
      d{0}
    {}

    int operator[](DieSide const *side) const {
      return d[side->index()];
    }

    int &operator[](DieSide const *side) {
      return d[side->index()];
    }

    bool contains(DieSide const *side) const {
      return d[side->index()] > 0;
    }

    int count() const {
      int n = 0;
      for (DieSide const *side : DieSide::ALL) {
        n += d[side->index()];
      }
      return n;
    }

    Score sum() const {
      int s = 0;
      for (DieSide const *side : DieSide::ALL) {
        s += d[side->index()] * side->score();
      }
      return s;
    }

  private:
    int d[DieSide::COUNT];
};

std::ostream &operator<<(std::ostream &out, Dice const &dice);
std::istream &operator>>(std::istream &in, Dice &dice);

#endif
