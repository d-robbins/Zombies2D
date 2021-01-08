#include "game.h"

Game::Game(const int& w, const int& h, const std::string& title)
	: width_(w), height_(h), name_(title) {
	window_.create(sf::VideoMode(width_, height_), name_);
	if (!game_font_.loadFromFile("arial.ttf")) {
		std::cout << "error opening font file\n";
	}
	window_.setFramerateLimit(60);

	player_.self_ =
		Entity(20.0f, sf::Color::Blue, sf::Vector2f(600.0f, 300.0f), true, 120.0f);

	player_.gun_.damage_ = 25.0f;
	player_.gun_.gname_ = "Glock-22";
	player_.gun_.bullets_ = 5000;

	intialize_game_board();

	intialize_texts(game_info, 24, sf::Text::Bold,
		sf::Color::Red);
}

void Game::intialize_game_board() {
	border_.resize(4);
	for (auto& wall : border_) {
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
			sf::Vector2f mpos = sf::Vector2f(event.mouseButton.x,
				event.mouseButton.y);

			sf::Vector2f dir =
				sf::Vector2f((mpos.x - current_dir.x), (mpos.y - current_dir.y));
			dir /= std::sqrt((powf(dir.x, 2.f) + powf(dir.y, 2.f)));
			std::cout << dir.x << " " << dir.y << std::endl;
			std::cout << "Bullet Queue: " << bullet_queue_.size() << std::endl;
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

	for (auto& zombie : zombies_) {
		window_.draw(zombie.self_.draw());
	}

	for (const auto& wall : border_) {
		window_.draw(wall);
	}

	for (const auto& bullet : bullet_queue_) {
		window_.draw(bullet.bbullet_);
	}

	window_.draw(format_game_stats(game_info));
	window_.display();
}

sf::Text& Game::format_game_stats(sf::Text& blank_text) {
	std::string printfo = "WEAPONS: FIRST: ";
	printfo += player_.gun_.gname_;
	printfo += " BULLETS LEFT: " + std::to_string(player_.gun_.bullets_);
	printfo += "\nGAME ROUND: " + std::to_string(game_round_);
	printfo += "\n\nPOINTS: " + std::to_string(player_.points_);
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
		float rand_speed = 100.0f; //(float)((rand() % 150) + 100);
		Zombie z;
		z.self_ = Entity(20.0f, sf::Color::Yellow,
			sf::Vector2f(30.0f, 30.f + (float)(60 * i)), false, rand_speed);
		zombies_.push_back(z);
	}
}

void Game::update_zombies() {
	// check for death
	
	bool zombies_alive = false;
	while (!zombies_alive) {
		int index = -1;
		for (size_t i = 0; i < zombies_.size(); ++i) {
			if (zombies_[i].health_ < 0.0f && zombies_.size() == 1) {
				zombies_.pop_back();
			}
			else if (zombies_[i].health_ < 0.0f) {
				index = i;
				break;
			}
		}

		if (index == -1) {
			zombies_alive = true;
		}
		else {
			zombies_[index] = zombies_.back();
			zombies_.pop_back();
		}
	}

	// move zombies
	
	sf::Vector2f direction;
	sf::Vector2f current_dir;
	sf::Vector2f player_dir = player_.self_.get_pos();
	for (auto& z : zombies_) {
		
		sf::Uint8 g = 255 - (2.5 * (100 - (int)z.health_));
		z.self_.set_color(255, g, 0);

		current_dir = z.self_.get_pos();

		direction = sf::Vector2f((player_dir.x - current_dir.x),
			(player_dir.y - current_dir.y));

		direction /= std::sqrt((powf(direction.x, 2.f) + powf(direction.y, 2.f)));
		z.self_.move(direction, border_, dt_);
	}
}

void Game::update_bullet_queue() {
	for (auto& b : bullet_queue_) {
		b.bbullet_.move(sf::Vector2f(b.dir_.x, b.dir_.y) * b.speed_ * dt_);
	}

	bool bullets_good = false;
	while (!bullets_good) {
		int index = -1;
		for (int i = 0; i < bullet_queue_.size(); ++i) {
			for (const auto& wall : border_) {
				if (bullet_queue_[i].bbullet_.getGlobalBounds().intersects(
					wall.getGlobalBounds())) {
					index = i;
					break;
				}
			}

			if (index != -1) break;
		}
		if (index != -1) {
			if (bullet_queue_.size() > 1) {
				bullet_queue_[index] = bullet_queue_.back();
				bullet_queue_.pop_back();
			}
			else {
				bullet_queue_.pop_back();
			}
		}
		else {
			bullets_good = true;
		}
	}

	detect_hit();
}

void Game::detect_hit() {
	for (auto& bullet : bullet_queue_) {
		for (auto& zom : zombies_) {
			if (bullet.bbullet_.getGlobalBounds().intersects(zom.self_.draw().getGlobalBounds())) {
				zom.health_ -= 5.0f;
				player_.points_ += 10;
			}
		}
	}
}

Game::~Game() {}

void Game::play() {
	window_.setKeyRepeatEnabled(false);

	while (window_.isOpen()) {
		update_dt();

		// poll for event updates
		poll();

		// process those event updates
		process_input();

		start_rounds();

		update_zombies();

		update_bullet_queue();

		// render to screen
		render();
	}
}

void Game::intialize_texts(sf::Text& text, const int& fontsz, sf::Uint32 style,
	const sf::Color& color) {
	text.setFont(game_font_);
	text.setCharacterSize(fontsz);
	text.setFillColor(color);
	text.setStyle(style);
}

void Game::process_input() {
	for (const auto& i : keys) {
		switch (i.first) {
		case (sf::Keyboard::Key::W):
			player_.self_.move(DIR::UP, border_, dt_);
			break;
		case (sf::Keyboard::Key::S):
			player_.self_.move(DIR::DOWN, border_, dt_);
			break;
		case (sf::Keyboard::Key::A):
			player_.self_.move(DIR::LEFT, border_, dt_);
			break;
		case (sf::Keyboard::Key::D):
			player_.self_.move(DIR::RIGHT, border_, dt_);
			break;
		default:
			break;
		}
	}
}

void Player::fire_gun(std::vector<Bullet>& queue, const sf::Vector2f& dir) {
	if (gun_.bullets_ > 0) {
		gun_.bullets_ -= 1;
		Bullet b;
		b.bbullet_ = sf::CircleShape(3.0f);
		b.bbullet_.setPosition(self_.get_pos());
		b.bbullet_.setFillColor(sf::Color::Red);
		b.speed_ = 1000.0f;
		b.dir_ = dir;

		queue.push_back(b);
	}
}
