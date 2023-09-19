#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "gem.h"

class Gem;

class Board
{
public:
  explicit Board(size_t x, size_t y, size_t width, size_t height);
  ~Board();
  
  void draw(sf::RenderWindow* window);
  void update(sf::Event event, int& score);
private:
  size_t x0, y0;
  size_t width, height;

  int collisionGems(int gi, int gj);
  sf::Vector2f calculPosition(size_t i, size_t j);
  void hoverEvent(sf::Event event, size_t ti, size_t tj, size_t& hi, size_t& hj, bool isTaken);
  bool pressedEvent(bool& isTaken, size_t& ti, size_t& tj, size_t hi, size_t hj);
  std::vector<std::vector<Gem*>> m_board;
};

