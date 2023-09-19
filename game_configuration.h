#pragma once

struct Game—onfiguration
{
  /*-------------- GEM SETTINGS ---------------*/
  char SPRITE_FILE[128] = "resources/gems.png";
  float ONE_SPRITE_WIDTH = 48.f;
  size_t GEM_SIZE = 50;

  /*------------- BOARD SETTINGS --------------*/
  size_t X0 = 20;
  size_t Y0 = 100;
  size_t GEMS_WIDTH_COUNT = 16;
  size_t GEMS_HEIGHT_COUNT = 9;

  /*------------- WINDOW SETTINGS -------------*/
  unsigned int WINDOW_WIDTH = 840;
  unsigned int WINDOW_HEIGHT = 570;
  size_t CLEAR_COLOR_RGB = 0x00003264;

  /*------------ MESSAGE SETTINGS -------------*/
  char FONT_FILE[128] = "resources/sansation.ttf";
  unsigned int CHARTER_SIZE = 30;
  float M_POSITION_X = 220;
  float M_POSITION_Y = 10;
  size_t M_COLOR_RGB = 0x00ffffff;

  /*-------------- GAME SETTINGS --------------*/
  int SCORE_GOAL = 300;
};

