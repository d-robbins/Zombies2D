#pragma once

#include <iostream>
#include <string>
#include <vector>
 
#include "entity.h"

const std::string PLAYER_TEX = "res/player.png";
const float MOVE_SCALE = 10.0f;

class Game
{
private:
	int round_ = 1;

	int width_, height_;
	std::string name_;

	sf::RenderWindow window_;
	Entity player_;

	std::vector<sf::RectangleShape> border_;
	
	void intialize_game();

public:
	Game(const int& w, const int& h, const std::string& title);
	~Game();
	
	void play();
};

