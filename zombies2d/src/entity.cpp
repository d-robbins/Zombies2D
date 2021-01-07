#include "entity.h"

Entity::Entity(const float& size, const sf::Color& color,
               const sf::Vector2f& pos, const bool& control) {
  body_ = sf::CircleShape(size);
  body_.setFillColor(color);
  body_.setPosition(pos);
  controllable_ = control;
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

void Entity::move(const sf::Vector2f& dir,
                  const std::vector<sf::RectangleShape>& map) {
  sf::CircleShape copy = body_;

  copy.move(dir);

  bool good_move = true;

  // collision detection
  for (const auto& wall : map) {
    if (copy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
      good_move = false;
      break;
    }
  }

  if (good_move) {
    body_.move(dir);
  }
}

void Entity::ez_switch_move(sf::Shape& target, const float& move_difference,
                            const DIR& direction) {
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
    default:
      break;
  }
}
