#include "board.h"
#include "bomb_gem.h"
#include "cross_gem.h"

#include <utility>
#include <cstdlib>
#include <cmath>

bool isThird(std::vector<std::vector<Gem*>> const& gems, size_t i, size_t j, ESprite color) {
  return
    (i >= 2) && (gems.at(i - 1).at(j)->getSprite() == color &&
      gems.at(i - 2).at(j)->getSprite() == color) ||
    (j >= 2) && (gems.at(i).at(j - 1)->getSprite() == color &&
      gems.at(i).at(j - 2)->getSprite() == color);
}

sf::Vector2f Board::calculPosition(size_t i, size_t j) {
  return sf::Vector2f(x0 + Gem::GEM_SIZE / 2.0f + j * Gem::GEM_SIZE,
    y0 + Gem::GEM_SIZE / 2.0f + i * Gem::GEM_SIZE);
}

Board::Board(size_t x0, size_t y0, size_t width, size_t height)
  : x0(x0), y0(y0), width(width), height(height)
{
  this->m_board.resize(height);
  for (size_t i = 0; i < height; i++) {
    this->m_board.at(i).resize(width);
    for (size_t j = 0; j < width; j++) {
      sf::Vector2f pos(calculPosition(i, j));
      ESprite color = static_cast<ESprite>(rand() % 4);

      while (isThird(m_board, i, j, color))
        color = static_cast<ESprite>(rand() % 4);

      this->m_board.at(i).at(j) = new Gem(pos.x, pos.y, color);
    }
  }
}

Board::~Board() {
  for (auto& line : this->m_board)
    for (auto& gem : line)
      delete gem;
}

void Board::draw(sf::RenderWindow* window) {
  for (auto const& line : this->m_board)
    for (auto const& gem : line)
        gem->draw(window);
}

void Board::hoverEvent(sf::Event event, size_t ti, size_t tj, size_t& hi, size_t& hj, bool isTaken) {
  size_t i = (event.mouseMove.y - y0) / Gem::GEM_SIZE;
  size_t j = (event.mouseMove.x - x0) / Gem::GEM_SIZE;

  if (i < height && j < width) {
    if (!isTaken || ti != hi || tj != hj)
      m_board.at(hi).at(hj)->hover(false);

    m_board.at(i).at(j)->hover(true);
    hi = i;
    hj = j;
  }
}

bool isNeighbor(size_t ti, size_t tj, size_t hi, size_t hj) {
  return (std::abs(static_cast<int>(ti - hi)) == 1 &&
    std::abs(static_cast<int>(tj - hj)) == 0) ||
    (std::abs(static_cast<int>(ti - hi)) == 0 &&
      std::abs(static_cast<int>(tj - hj)) == 1);
}

void SwapGems(std::vector<std::vector<Gem*>>& gems, size_t i1, size_t j1, size_t i2, size_t j2) {
  sf::Vector2f pos = gems.at(i1).at(j1)->getPosition();
  gems.at(i1).at(j1)->setPosition(gems.at(i2).at(j2)->getPosition());
  gems.at(i2).at(j2)->setPosition(pos);
  std::swap(gems.at(i1).at(j1), gems.at(i2).at(j2));
}

bool Board::pressedEvent(bool& isTaken, size_t& ti, size_t& tj, size_t hi, size_t hj) {
  bool isSwapped = false;

  if (!isTaken) {
    ti = hi;
    tj = hj;
  }
  else if (isNeighbor(ti, tj, hi, hj)) {
    SwapGems(m_board, ti, tj, hi, hj);
    isSwapped = true;
  }

  isTaken = !isTaken;
  m_board.at(ti).at(tj)->hover(isTaken);
  m_board.at(hi).at(hj)->hover(isTaken);
  return isSwapped;
}

int Board::collisionGems(int gi, int gj) {
  ESprite color = m_board.at(gi).at(gj)->getSprite();
  int gemsCount = 0, lineCount = 0, score = 0;
  int left = gj - 1, right = gj + 1;
  int up = gi - 1, down = gi + 1;

  while (left >= 0 && (m_board.at(gi).at(left)->getSprite() == color))
    --left;

  while (right < width && (m_board.at(gi).at(right)->getSprite() == color))
    ++right;

  while (up >= 0 && (m_board.at(up).at(gj)->getSprite() == color))
    --up;

  while (down < height && (m_board.at(down).at(gj)->getSprite() == color))
    ++down;

  if (right - left > 3) {
    lineCount++;
    gemsCount += right - left - 1;
    for (int j = left + 1; j < right; j++)
      score += m_board.at(gi).at(j)->activate(m_board, gi, j, width, height);
  }

  if (down - up > 3) {
    lineCount++;
    gemsCount += down - up - 1;
    for (int i = up + 1; i < down; i++)
      score += m_board.at(i).at(gj)->activate(m_board, i, gj, width, height);
  }

  if (lineCount == 2) {
    sf::Vector2f pos(calculPosition(gi, gj));
    delete m_board.at(gi).at(gj);
    m_board.at(gi).at(gj) = new BombGem(pos.x, pos.y, color);
  }
  else if (gemsCount >= 5) {
    sf::Vector2f pos(calculPosition(gi, gj));
    delete m_board.at(gi).at(gj);
    m_board.at(gi).at(gj) = new CrossGem(pos.x, pos.y, static_cast<ESprite>(rand() % 4), color);
  }

  return score;
}

void updateGems(std::vector<std::vector<Gem*>>& gems, size_t width, size_t height) {
  int find;
  for (int i = static_cast<int>(height - 1); i >= 0; i--) {
    for (int j = 0; j < width; j++) {
      if (gems.at(i).at(j)->getSprite() == ESprite::S_NONE) {
        find = i - 1;

        while (find >= 0 && gems.at(find).at(j)->getSprite() == ESprite::S_NONE)
          --find;

        if (find >= 0)
          SwapGems(gems, i, j, find, j);
        else
          gems.at(i).at(j)->setSprite(static_cast<ESprite>(rand() % 4));
      }
    }
  }
}

void Board::update(sf::Event event, int& score) {
  static bool isUpdated = true;
  static bool isTaken = false;
  static bool isSwapped = false;
  static size_t hover_i = 0, hover_j = 0;
  static size_t taken_i = 0, taken_j = 0;
  int dscore = 0;

  switch (event.type)
  {
  case sf::Event::MouseMoved:
    hoverEvent(event, taken_i, taken_j, hover_i, hover_j, isTaken);
    break;

  case sf::Event::MouseButtonPressed:
    isSwapped = pressedEvent(isTaken, taken_i, taken_j, hover_i, hover_j);
    break;

  default:
    break;
  }

  if (isSwapped) {
    dscore += collisionGems(static_cast<int>(hover_i), static_cast<int>(hover_j));
    dscore += collisionGems(static_cast<int>(taken_i), static_cast<int>(taken_j));

    if (dscore == 0)
      SwapGems(m_board, taken_i, taken_j, hover_i, hover_j);
    else
      isUpdated = false;

    score += dscore;
    isSwapped = false;
  }

  if (!isUpdated) {
    updateGems(m_board, width, height);
    isUpdated = true;

    for (size_t i = 0; i < height; i++) {
      for (size_t j = 0; j < width; j++) {
        ESprite color = m_board.at(i).at(j)->getSprite();
        if (color != ESprite::S_NONE && isThird(m_board, i, j, color)) {
          score += collisionGems(static_cast<int>(i), static_cast<int>(j));
          isUpdated = false;
        }
      }
    }
  }
}
