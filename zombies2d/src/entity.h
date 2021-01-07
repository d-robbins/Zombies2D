#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <unordered_map>

enum class DIR { UP, DOWN, LEFT, RIGHT };

const float MOVE_DIF = 10.0f;

class Entity {
 private:
  bool controllable_;
  sf::CircleShape body_;

  void ez_switch_move(sf::Shape& target, const float& move_difference,
                      const DIR& direction);

 public:
  Entity(const float& size, const sf::Color& color, const sf::Vector2f& pos,
         const bool& control);
  Entity() {}
  ~Entity() {}

  void move(const DIR& dir, const std::vector<sf::RectangleShape>& map);
  void move(const sf::Vector2f& dir,
            const std::vector<sf::RectangleShape>& map);

  sf::CircleShape& draw() { return body_; }

  bool controllable() const { return controllable_; }
  void controllable(const bool& val) { controllable_ = val; }

  bool intersects(const sf::CircleShape& target) {
    if (this->body_.getGlobalBounds().intersects(target.getGlobalBounds()))
      return true;
  }

  float get_x() const { return body_.getPosition().x; }
  float get_y() const { return body_.getPosition().y; }
  sf::Vector2f get_pos() const { return body_.getPosition(); }

  void set_x(const sf::Vector2f& pos) { body_.setPosition(pos); }
};
