#include "entity.h"

Entity::Entity(const float& size, const sf::Color& color,
               const sf::Vector2f& pos, const bool& control,
               const float& speed) {
  body_ = sf::CircleShape(size);
  body_.setFillColor(color);
  body_.setPosition(pos);
  controllable_ = control;
  speed_ = speed;
}

void Entity::move(const DIR& dir, const std::vector<sf::RectangleShape>& map,
                  const float& dt) {
  sf::CircleShape copy = body_;

  ez_switch_move(copy, dt, dir);

  bool good_move = true;

  // collision detection
  for (const auto& wall : map) {
    if (copy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
      good_move = false;
      break;
    }
  }

  if (good_move) {
    ez_switch_move(body_, dt, dir);
  }
}

void Entity::move(const sf::Vector2f& dir,
                  const std::vector<sf::RectangleShape>& map, const float& dt) {
  sf::CircleShape copy = body_;

  copy.move(dir * speed_ * dt);

  bool good_move = true;

  // collision detection
  for (const auto& wall : map) {
    if (copy.getGlobalBounds().intersects(wall.getGlobalBounds())) {
      good_move = false;
      break;
    }
  }

  if (good_move) {
    body_.move(dir * speed_ * dt);
  }
}

void Entity::ez_switch_move(sf::Shape& target, const float& dt,
                            const DIR& direction) {
  switch (direction) {
    case DIR::UP:
      target.move(sf::Vector2f(0.0f, -1.f) * speed_ * dt);
      break;
    case DIR::DOWN:
      target.move(sf::Vector2f(0.0f, 1.f) * speed_ * dt);
      break;
    case DIR::LEFT:
      target.move(sf::Vector2f(-1.f, 0.0f) * speed_ * dt);
      break;
    case DIR::RIGHT:
      target.move(sf::Vector2f(1.f, 0.0f) * speed_ * dt);
      break;
    default:
      break;
  }
}
