#include "game.h"

Game::Game(const int &w, const int &h, const std::string &title)
    : width_(w),
      height_(h),
      name_(title),
      weapons_(std::make_pair(Gun(), Gun())) {
  window_.create(sf::VideoMode(width_, height_), name_);
  if (!game_font_.loadFromFile("arial.ttf")) {
    std::cout << "error opening font file\n";
  }
  window_.setFramerateLimit(60);

  mStatisticsText.setFont(game_font_);
  mStatisticsText.setCharacterSize(24);
  mStatisticsText.setFillColor(sf::Color::Red);
  mStatisticsText.setStyle(sf::Text::Bold | sf::Text::Underlined);

  player_.self_ =
      Entity(20.0f, sf::Color::Red, sf::Vector2f(30.0f, 30.0f), true);
  intialize_game_board();

  intialize_texts(game_info, 24, sf::Text::Bold | sf::Text::Underlined,
                  sf::Color::Red);
  intialize_texts(mStatisticsText, 24, sf::Text::Bold | sf::Text::Underlined,
                  sf::Color::Red);
}

void Game::intialize_game_board() {
  border_.resize(4);
  for (auto &wall : border_) {
    wall.setFillColor(sf::Color::Red);
  }

  // ugly code
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

void Game::poll() {
  sf::Event event;
  while (window_.pollEvent(event)) {
    if (event.type == sf::Event::Closed) window_.close();

    if (event.type == sf::Event::EventType::KeyPressed) {
      if (keys.count(event.key.code) == 0) {
        keys[event.key.code] = true;
        changedKeys.push_back(event.key.code);
      }
    }
    if (event.type == sf::Event::EventType::KeyReleased) {
      if (keys.count(event.key.code) == 1) {
        keys.erase(event.key.code);
        changedKeys.push_back(event.key.code);
      }
    }
  }
}

void Game::render() {
  // sf::Transform t3(2.f, 0.f, 10.f, 0.f, 1.f, 50.f, 0.f, 0.f, 1.f);

  window_.clear();
  window_.draw(player_.self_.draw());

  for (const auto &wall : border_) {
    window_.draw(wall);
  }

  window_.draw(format_gun_stats(game_info));
  window_.draw(mStatisticsText);
  window_.display();
}

sf::Text &Game::format_gun_stats(sf::Text &blank_text) {
  std::string printfo = "\t\t\tWEAPONS: FIRST: ";

  printfo += weapons_.first.get_name();
  printfo += " SECOND: ";
  printfo += weapons_.second.get_name();

  blank_text.setString(printfo);

  return blank_text;
}

Game::~Game() {}

void Game::play() {
  window_.setKeyRepeatEnabled(false);

  // (1) see todo in updateStatistics()
  sf::Clock clock;
  sf::Time timeSinceLastUpdate = sf::Time::Zero;

  while (window_.isOpen()) {
    sf::Time elapsedTime = clock.restart();
    timeSinceLastUpdate += elapsedTime;

    while (timeSinceLastUpdate > TimePerFrame) {
      timeSinceLastUpdate -= TimePerFrame;

      poll();

      process_input();
    }

    updateStatistics(TimePerFrame);
    render();
  }
}

Gun::Gun(const std::string &name, const std::string &bullet_type,
         const int &mag, const double &dmg)
    : gun_name_(name),
      bullet_type_(bullet_type),
      mag_size_(mag),
      bullet_damage_(dmg) {}

void Game::updateStatistics(sf::Time dt) {
  // (1) TODO: understand this code better
  mStatisticsUpdateTime += dt;
  mStatisticsNumFrames += 1;
  if (mStatisticsUpdateTime >= sf::seconds(1.0f)) {
    mStatisticsText.setString("FPS: " + std::to_string(mStatisticsNumFrames));

    mStatisticsUpdateTime -= sf::seconds(1.0f);
    mStatisticsNumFrames = 0;
  }
}

void Game::intialize_texts(sf::Text &text, const int &fontsz, sf::Uint32 style,
                           const sf::Color &color) {
  text.setFont(game_font_);
  text.setCharacterSize(fontsz);
  text.setFillColor(color);
  text.setStyle(style);
}

void Game::process_input() {
  for (const auto &i : keys) {
    switch (i.first) {
      case (sf::Keyboard::Key::W):
        player_.self_.move(DIR::UP, border_);
        break;
      case (sf::Keyboard::Key::S):
        player_.self_.move(DIR::DOWN, border_);
        break;
      case (sf::Keyboard::Key::A):
        player_.self_.move(DIR::LEFT, border_);
        break;
      case (sf::Keyboard::Key::D):
        player_.self_.move(DIR::RIGHT, border_);
        break;
      default:
        break;
    }
  }
}
