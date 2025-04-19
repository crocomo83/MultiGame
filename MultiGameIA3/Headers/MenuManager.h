#pragma once

#include "../Headers/Menu.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

class MenuManager {

public:
	MenuManager(sf::RenderWindow* window_);
	~MenuManager();

	void setMainMenu();
	void setChessMenu();
	void setChessMenuVSComputer();
	void setPower4Menu();
	void setPower4MenuVSComputer();
	void run();

private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
	sf::RenderWindow* window;
	Menu* menu;
};