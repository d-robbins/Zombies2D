#include "game.h"

Game::Game(const int &w, const int &h, const std::string &title)
    : width_(w), height_(h), name_(title) {
  window_.create(sf::VideoMode(width_, height_), name_);
  if (!game_font_.loadFromFile("arial.ttf")) {
    std::cout << "error opening font file\n";
  }
  window_.setFramerateLimit(60);

  player_.self_ =
      Entity(20.0f, sf::Color::Red, sf::Vector2f(600.0f, 300.0f), true);

  player_.gun_.damage_ = 10.0f;
  player_.gun_.gname_ = "Glock-22";
  player_.gun_.bullets_ = 30;

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

    if (event.type == sf::Event::MouseButtonPressed) {
      sf::Vector2f current_dir = player_.self_.get_pos();
      sf::Vector2f mpos = sf::Vector2f((float)sf::Mouse::getPosition().x,
                                       (float)sf::Mouse::getPosition().y);

      sf::Vector2f dir =
          sf::Vector2f((current_dir.x - mpos.x), (current_dir.y - mpos.y));
      std::cout << dir.x << " " << dir.y << std::endl;
      player_.fire_gun(bullet_queue_, dir);
    }

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

  for (auto &zombie : zombies_) {
    window_.draw(zombie.self_.draw());
  }

  for (const auto &wall : border_) {
    window_.draw(wall);
  }

  for (const auto &bullet : bullet_queue_) {
    window_.draw(bullet.bbullet_);
  }

  window_.draw(format_gun_stats(game_info));
  window_.draw(mStatisticsText);
  window_.display();
}

sf::Text &Game::format_gun_stats(sf::Text &blank_text) {
  std::string printfo = "\t\t\tWEAPONS: FIRST: ";
  printfo += player_.gun_.gname_;
  blank_text.setString(printfo);
  return blank_text;
}

void Game::start_rounds() {
  if (zombies_.size() == 0) {
    game_round_++;
    spawn_zombies();
  }
}

void Game::spawn_zombies() {
  int num_to_spawn;
  srand(time(0));
  num_to_spawn = rand() % (3 * game_round_) + game_round_;
  std::cout << "Game Round: " << game_round_ << std::endl;
  std::cout << "Spawning: " << num_to_spawn << std::endl;
  for (int i = 0; i < num_to_spawn; i++) {
    Zombie z;
    z.self_ = Entity(20.0f, sf::Color::Yellow,
                     sf::Vector2f(30.0f, 30.f + (float)(60 * i)), false);
    zombies_.push_back(z);
  }
}

void Game::update_zombies() {
  sf::Vector2f direction;
  sf::Vector2f current_dir;
  sf::Vector2f player_dir = player_.self_.get_pos();
  for (auto &z : zombies_) {
    current_dir = z.self_.get_pos();
    direction = sf::Vector2f((player_dir.x - current_dir.x),
                             (player_dir.y - current_dir.y));
    z.self_.move(direction, border_);
  }
}

void Game::update_bullet_queue() {
  for (auto &b : bullet_queue_) {
    b.bbullet_.move(b.dir_.x / 100.0f, b.dir_.y / 100.0f);
  }
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

      // poll for event updates
      poll();

      // process those event updates
      process_input();
    }

    start_rounds();

    update_zombies();

    update_bullet_queue();

    // update time & statistics stuff
    updateStatistics(TimePerFrame);

    // render to screen
    render();
  }
}

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

void Player::fire_gun(std::vector<Bullet> &queue, const sf::Vector2f &dir) {
  if (gun_.bullets_ > 0) {
    gun_.bullets_ -= 1;
    Bullet b;
    b.bbullet_ = sf::CircleShape(3.0f);
    b.bbullet_.setPosition(self_.get_pos());
    b.bbullet_.setFillColor(sf::Color::Red);
    b.dir_ = dir;

    queue.push_back(b);
  }
}
