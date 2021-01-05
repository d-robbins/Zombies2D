#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <utility>
 
#include "entity.h"

const std::string PLAYER_TEX = "res/player.png";
const float MOVE_SCALE = 10.0f;
const float ZOMBIE_SIZE = 10.0f;

struct Bullet {
	sf::CircleShape bbullet_;
	std::string btype_;
};

class Gun {
private:
	std::string gun_name_;
	std::string bullet_type_;
	int mag_size_;
	double bullet_damage_;

public:
	Gun(const std::string& name, const std::string& bullet_type, const int& mag, const double& dmg);
	Gun() : gun_name_("n/a"), bullet_type_("n/a"), mag_size_(-1), bullet_damage_(-1) {}
	~Gun() {}

	// load guns from file

	std::string get_name() const { return gun_name_; }
};

// TODO: make zombies entities

//class Zombie {
//private:
//	sf::CircleShape body_;
//	float health_;
//public:
//	Zombie(const sf::Vector2f& pos, const float& hp);
//	~Zombie() {}
//
//	void update(const Entity& player);
//};

class Game
{
private:
	int round_ = 1;
	int selection_ = 0;

	int width_, height_;
	std::string name_;

	sf::RenderWindow window_;
	sf::Font game_font_;

	Entity player_;

	std::vector<sf::RectangleShape> border_;

	std::pair<Gun, Gun> weapons_;
	
	void intialize_game_board();
	sf::Text& format_gun_stats(sf::Text& blank_text);

public:
	Game(const int& w, const int& h, const std::string& title);
	~Game();
	
	void play();
};

