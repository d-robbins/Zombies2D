#include "entity.h"

Entity::Entity(const float& size, const sf::Color& color, const sf::Vector2f& pos, const bool& control)
{
	body_ = sf::CircleShape(size);
	body_.setFillColor(color);
	body_.setPosition(pos);
	controllable_ = control;
}

void Entity::update(const sf::Event& e, const std::vector<sf::RectangleShape>& map) {
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

	switch (dir) {
	case DIR::UP:
		copy.move(sf::Vector2f(0.0f, -MOVE_DIF));
		break;
	case DIR::DOWN:
		copy.move(sf::Vector2f(0.0f, MOVE_DIF));
		break;
	case DIR::LEFT:
		copy.move(sf::Vector2f(-MOVE_DIF, 0.0f));
		break;
	case DIR::RIGHT:
		copy.move(sf::Vector2f(MOVE_DIF, 0.0f));
		break;
	default: break;
	}

	bool good_move = true;

	for (const auto& wall : map) {
		if (copy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
			good_move = false;
			break;
		}
	}
	
	if (good_move) {
		switch (dir) {
		case DIR::UP:
			body_.move(sf::Vector2f(0.0f, -MOVE_DIF));
			break;
		case DIR::DOWN:
			body_.move(sf::Vector2f(0.0f, MOVE_DIF));
			break;
		case DIR::LEFT:
			body_.move(sf::Vector2f(-MOVE_DIF, 0.0f));
			break;
		case DIR::RIGHT:
			body_.move(sf::Vector2f(MOVE_DIF, 0.0f));
			break;
		default: break;
		}
	}
}
