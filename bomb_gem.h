#pragma once
#include "gem.h"

class BombGem : public Gem
{
public:
  BombGem(float x, float y, ESprite sprite);

  virtual void setPosition(sf::Vector2f pos) override;
  virtual void draw(sf::RenderWindow* window) const override;
  virtual int activate(std::vector<std::vector<Gem*>>& gems, size_t i, size_t j, size_t width, size_t height) override;
private:
  sf::RectangleShape m_bomb;
};

