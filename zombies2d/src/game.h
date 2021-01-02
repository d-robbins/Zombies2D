#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

const std::string PLAYER_TEX = "res/player.png";
const float MOVE_SCALE = 10.0f;

class Game
{
private:
	int width_, height_;
	std::string name_;

	sf::RenderWindow window_;
	sf::CircleShape player_;
	sf::Texture player_texture_;
	
	std::vector<sf::RectangleShape> border_;
	void intialize_game();
	void process_input(const sf::Event& e);
public:
	Game(const int& w, const int& h, const std::string& title);
	~Game();
	
	void play();
};

