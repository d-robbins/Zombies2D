#pragma once

// (1) framerate stuff
// https://github.com/SFML/SFML-Game-Development-Book/blob/master/07_Gameplay/Source/Application.cpp

#include <SFML/System/Time.hpp>
#include <iostream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "entity.h"

const sf::Time TimePerFrame = sf::seconds(1.f / 60.f);
const float MOVE_SCALE = 10.0f;
const float ZOMBIE_SIZE = 10.0f;

struct Bullet {
  sf::CircleShape bbullet_;
  std::string btype_;
};

struct Gun {};

struct Player {
  Entity self_;
};

class Game {
 private:
  int width_, height_;
  std::string name_;

  sf::RenderWindow window_;
  sf::Font game_font_;

  sf::Text game_info;

  sf::Text mStatisticsText;
  sf::Time mStatisticsUpdateTime;
  std::size_t mStatisticsNumFrames;

  Player player_;

  std::unordered_map<int, bool> keys;
  std::list<int> changedKeys;

  std::vector<sf::RectangleShape> border_;

  void intialize_game_board();
  void poll();
  void render();

  void updateStatistics(sf::Time dt);

  void intialize_texts(sf::Text& text, const int& fontsz, sf::Uint32 style,
                       const sf::Color& color);

  void process_input();

  sf::Text& format_gun_stats(sf::Text& blank_text);

 public:
  Game(const int& w, const int& h, const std::string& title);
  ~Game();

  void play();
};
