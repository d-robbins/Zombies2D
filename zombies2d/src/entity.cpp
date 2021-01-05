#include "entity.h"

Entity::Entity(const float& size, const sf::Color& color, const sf::Vector2f& pos, const bool& control)
{
	body_ = sf::CircleShape(size);
	body_.setFillColor(color);
	body_.setPosition(pos);
	controllable_ = control;
}

void Entity::update(const sf::Event& e, const std::vector<sf::RectangleShape>& map) {
	// nest with if (controllable)
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		move(DIR::UP, map);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		move(DIR::DOWN, map);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		move(DIR::LEFT, map);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		move(DIR::RIGHT, map);
	}
}

void Entity::move(const DIR& dir, const std::vector<sf::RectangleShape>& map) {
	sf::CircleShape copy = body_;

	ez_switch_move(copy, MOVE_DIF, dir);

	bool good_move = true;

	// collision detection
	for (const auto& wall : map) {
		if (copy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
			good_move = false;
			break;
		}
	}

	if (good_move) {
		ez_switch_move(body_, MOVE_DIF, dir);
	}
}


void Entity::ez_switch_move(sf::Shape& target, const float& move_difference, const DIR& direction) {
	switch (direction) {
	case DIR::UP:
		target.move(sf::Vector2f(0.0f, -move_difference));
		break;
	case DIR::DOWN:
		target.move(sf::Vector2f(0.0f, move_difference));
		break;
	case DIR::LEFT:
		target.move(sf::Vector2f(-move_difference, 0.0f));
		break;
	case DIR::RIGHT:
		target.move(sf::Vector2f(move_difference, 0.0f));
		break;
	default: break;
	}
}