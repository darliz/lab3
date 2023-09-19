#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

enum class ESprite {
  S_RED = 0,
  S_GREEN,
  S_YELLOW,
  S_PURPLE,
  S_NONE
};

enum class EBonusSprite {
  BS_RED_CROSS = 0,
  BS_GREEN_CROSS,
  BS_YELLOW_CROSS,
  BS_PURPLE_CROSS,
  BS_BOMB
};

enum class EGemScore {
  GS_GEM = 1,
  GS_ÑROSS = 5,
  GS_BOMB = 10
};

class Gem
{
public:
  static sf::Texture SPRITE;
  static float ONE_SPRITE_WIDTH;
  static size_t GEM_SIZE;

  Gem(float x, float y, ESprite sprite);
  
  ESprite getSprite() const;
  sf::Vector2f getPosition() const;

  void setSprite(ESprite sprite);
  void hover(bool isHover);

  virtual void setPosition(sf::Vector2f pos);
  virtual void draw(sf::RenderWindow* window) const;
  virtual int activate(std::vector<std::vector<Gem*>>& gems, size_t i, size_t j, size_t width, size_t height);

private:
  ESprite m_sprite;
  sf::RectangleShape m_gem;
};

