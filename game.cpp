#include "game.h"
#include <cstdlib>
#include <string>

static size_t BOARD_X0;
static size_t BOARD_Y0;
static size_t GEMS_WIDTH_COUNT;
static size_t GEMS_HEIGHT_COUNT;

static unsigned int WINDOW_WIDTH;
static unsigned int WINDOW_HEIGHT;
static sf::Color CLEAR_COLOR;

static sf::Font FONT;
static unsigned int CHARTER_SIZE;
static float M_POSITION_X ;
static float M_POSITION_Y;
static sf::Color M_COLOR;

static int SCORE_GOAL;

Game::Game()
  :m_window(nullptr), m_board(nullptr), m_score(0)
{}

Game::~Game() {
  delete m_board;
  delete m_window;
}

bool Game::setConfiguration(Game—onfiguration const& cfg) {
  sf::Uint8 r, g, b;
  
  if (!Gem::SPRITE.loadFromFile(cfg.SPRITE_FILE))
    return false;

  if (!FONT.loadFromFile(cfg.FONT_FILE))
    return false;

  srand(static_cast<unsigned int>(time(NULL)));
  
  Gem::ONE_SPRITE_WIDTH = cfg.ONE_SPRITE_WIDTH;
  Gem::GEM_SIZE = cfg.GEM_SIZE;

  BOARD_X0 = cfg.X0;
  BOARD_Y0 = cfg.Y0;
  GEMS_WIDTH_COUNT = cfg.GEMS_WIDTH_COUNT;
  GEMS_HEIGHT_COUNT = cfg.GEMS_HEIGHT_COUNT;

  WINDOW_WIDTH = cfg.WINDOW_WIDTH;
  WINDOW_HEIGHT = cfg.WINDOW_HEIGHT;

  r = static_cast<sf::Uint8>((cfg.CLEAR_COLOR_RGB & 0x00ff0000) >> 16);
  g = static_cast<sf::Uint8>((cfg.CLEAR_COLOR_RGB & 0x0000ff00) >> 8);
  b = static_cast<sf::Uint8>(cfg.CLEAR_COLOR_RGB & 0x000000ff);
  CLEAR_COLOR = sf::Color(r, g, b);

  CHARTER_SIZE = cfg.CHARTER_SIZE;
  M_POSITION_X = cfg.M_POSITION_X;
  M_POSITION_Y = cfg.M_POSITION_Y;

  r = static_cast<sf::Uint8>((cfg.M_COLOR_RGB & 0x00ff0000) >> 16);
  g = static_cast<sf::Uint8>((cfg.M_COLOR_RGB & 0x0000ff00) >> 8);
  b = static_cast<sf::Uint8>(cfg.M_COLOR_RGB & 0x000000ff);
  M_COLOR = sf::Color(r, g, b);

  SCORE_GOAL = cfg.SCORE_GOAL;

  return true;
}

void setMessage(sf::Text& message, int score) {
  std::string msg("Score: ");
  msg += std::to_string(score);
  msg += "\t\t\t\tGoal: ";
  msg += std::to_string(SCORE_GOAL);
  message.setString(msg);
}

bool Game::init(Game—onfiguration const& cfg) {
  if (!setConfiguration(cfg))
    return false;
  
  m_score = 0;

  m_window = new sf::RenderWindow(
    sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32U),
    "Gems", sf::Style::Titlebar | sf::Style::Close
  );

  m_window->setVerticalSyncEnabled(true);

  m_message.setFont(FONT);
  m_message.setCharacterSize(CHARTER_SIZE);
  m_message.setPosition(M_POSITION_X, M_POSITION_Y);
  m_message.setFillColor(M_COLOR);
  setMessage(m_message, m_score);

  m_board = new Board(BOARD_X0, BOARD_Y0, GEMS_WIDTH_COUNT, GEMS_HEIGHT_COUNT);

  return true;
}

bool isCloseEvent(sf::Event event) {
  return ((event.type == sf::Event::Closed) ||
    ((event.type == sf::Event::KeyPressed) &&
      (event.key.code == sf::Keyboard::Escape)));
}

void Game::loop() {

  bool isOver = false;
  while (m_window->isOpen()) {
    sf::Event event;

    while (m_window->pollEvent(event)) {
      if (isCloseEvent(event)) {
        m_window->close();
        break;
      }

      if (isOver && (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Space) )
      {
        m_score = 0;
        delete m_board;
        m_board = new Board(BOARD_X0, BOARD_Y0, GEMS_WIDTH_COUNT, GEMS_HEIGHT_COUNT);
        setMessage(m_message, m_score);
        isOver = false;
      }

      if (!isOver)
        m_board->update(event, m_score);
    }

    if (!isOver) {
      m_board->update(sf::Event(), m_score);
      setMessage(m_message, m_score);
      m_window->clear(CLEAR_COLOR);

      if (m_score > SCORE_GOAL) {
        isOver = true;
        m_message.setString("Game Over! Goal achieved!\n Space to start new game.");
      }


      m_window->draw(m_message);
      m_board->draw(m_window);
      m_window->display();
    }
  }
}
