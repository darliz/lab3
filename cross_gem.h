#pragma once
#include "gem.h"

class CrossGem : public Gem
{
public:
  CrossGem(float x, float y, ESprite sprite, ESprite bonus);

  virtual void setPosition(sf::Vector2f pos) override;
  virtual void draw(sf::RenderWindow* window) const override;
  virtual int activate(std::vector<std::vector<Gem*>>& gems, size_t i, size_t j, size_t width, size_t height) override;
private:
  ESprite m_bonus;
  sf::RectangleShape m_cross;
};

