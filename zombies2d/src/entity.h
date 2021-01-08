#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

enum class DIR { UP, DOWN, LEFT, RIGHT };

class Entity {
 private:
  bool controllable_;
  sf::CircleShape body_;

  void ez_switch_move(sf::Shape& target, const float& dt, const DIR& direction);

  float speed_;

 public:
  Entity(const float& size, const sf::Color& color, const sf::Vector2f& pos,
         const bool& control, const float& speed);
  Entity() {}
  ~Entity() {}

  void move(const DIR& dir, const std::vector<sf::RectangleShape>& map,
            const float& dt);
  void move(const sf::Vector2f& dir, const std::vector<sf::RectangleShape>& map,
            const float& dt);

  sf::CircleShape& draw() { return body_; }

  bool controllable() const { return controllable_; }
  void controllable(const bool& val) { controllable_ = val; }

  void speed(const float& sp) { speed_ = sp; }

  bool intersects(const sf::CircleShape& target) {
    if (this->body_.getGlobalBounds().intersects(target.getGlobalBounds()))
      return true;
  }

  float get_x() const { return body_.getPosition().x; }
  float get_y() const { return body_.getPosition().y; }
  sf::Vector2f get_pos() const { return body_.getPosition(); }

  void set_x(const sf::Vector2f& pos) { body_.setPosition(pos); }
};
