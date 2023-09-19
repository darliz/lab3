#include <SFML/Graphics.hpp>
#include <cmath>
#include <ctime>
#include <cstdlib>
#include "game.h"

int main() {
  Game game;

  if (!game.init(Game—onfiguration()))
    return EXIT_FAILURE;

  game.loop();
  return 0;
}
