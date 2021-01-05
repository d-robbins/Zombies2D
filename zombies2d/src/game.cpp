#include "game.h"

Game::Game(const int& w, const int& h, const std::string& title)
    : width_(w), height_(h), name_(title), weapons_(std::make_pair(Gun(), Gun())) {
  window_.create(sf::VideoMode(width_, height_), name_);
  if (!game_font_.loadFromFile("arial.ttf")) {
    std::cout << "error opening font file\n";
  }

  player_ = Entity(20.0f, sf::Color::Red, sf::Vector2f(30.0f, 30.0f), true);
  intialize_game_board();
}

void Game::intialize_game_board() {
  border_.resize(4);
  for (auto& wall : border_) {
    wall.setFillColor(sf::Color::Red);
  }

  border_[0] =
      sf::RectangleShape(sf::Vector2f(static_cast<float>(width_), 30.f));
  border_[1] =
      sf::RectangleShape(sf::Vector2f(static_cast<float>(width_), 30.f));
  border_[2] =
      sf::RectangleShape(sf::Vector2f(static_cast<float>(height_ - 60), 30.f));
  border_[3] =
      sf::RectangleShape(sf::Vector2f(static_cast<float>(height_ - 60), 30.f));

  border_[0].setFillColor(sf::Color::Green);

  border_[1].setPosition(
      sf::Vector2f(0.0f, static_cast<float>(height_) - 30.0f));
  border_[1].setFillColor(sf::Color::Green);

  border_[2].rotate(90.0f);
  border_[3].rotate(90.0f);
  border_[2].setFillColor(sf::Color::Green);
  border_[3].setFillColor(sf::Color::Green);

  border_[2].setPosition(sf::Vector2f(30.0f, 30.0f));
  border_[3].setPosition(sf::Vector2f(static_cast<float>(width_), 30.0f));
}

sf::Text& Game::format_gun_stats(sf::Text& blank_text) {
    blank_text.setFont(game_font_);
    std::string printfo = "WEAPONS: FIRST: ";

    printfo += weapons_.first.get_name();
    printfo += " SECOND: ";
    printfo += weapons_.second.get_name();

    blank_text.setString(printfo);
    blank_text.setCharacterSize(24);
    blank_text.setFillColor(sf::Color::Red);
    blank_text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    return blank_text;
}

Game::~Game() {}

void Game::play() {
  while (window_.isOpen()) {
    sf::Text game_info;
    sf::Event event;
    while (window_.pollEvent(event)) {
      if (event.type == sf::Event::Closed) window_.close();

      player_.update(event, border_);
    }

    window_.clear();
    window_.draw(player_.draw());

    for (const auto& wall : border_) {
      window_.draw(wall);
    }

    window_.draw(format_gun_stats(game_info));

    window_.display();
  }
}

//Zombie::Zombie(const sf::Vector2f& pos, const float& hp) {
//  this->body_ = sf::CircleShape(ZOMBIE_SIZE);
//  this->body_.setPosition(pos);
//  this->health_ = hp;
//}
//
//void Zombie::update(const Entity& player) {}

Gun::Gun(const std::string& name, const std::string& bullet_type,
         const int& mag, const double& dmg)
    : gun_name_(name),
      bullet_type_(bullet_type),
      mag_size_(mag),
      bullet_damage_(dmg) {}
