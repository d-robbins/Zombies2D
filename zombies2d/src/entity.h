#pragma once

#include <iostream>
#include <SFML/Graphics.hpp>

enum class DIR {UP, DOWN, LEFT, RIGHT};

const float MOVE_DIF = 20.0f;

class Entity {
private:
	bool controllable_;
	sf::CircleShape body_;
	
	void move(const DIR& dir, const std::vector<sf::RectangleShape>& map);

public:
	Entity(const float& size, const sf::Color& color, const sf::Vector2f& pos, const bool& control);
	Entity() = default;
	~Entity() {}

	void update(const sf::Event& e, const std::vector<sf::RectangleShape>& map);

	sf::CircleShape& draw() { return body_; }

	bool controllable() const { return controllable_; }
	void controllable(const bool& val) { controllable_ = val; }

	float get_x() const { return body_.getPosition().x; }
	float get_y() const { return body_.getPosition().y; }
	sf::Vector2f get_pos() const { return body_.getPosition(); }

	void set_x(const sf::Vector2f& pos) { body_.setPosition(pos); }
};

