#pragma once

#include "../Headers/Menu.h"

#include <SFML/Graphics.hpp>

class MenuManager {

public:
	MenuManager(sf::RenderWindow* window_);
	~MenuManager();

	void setMainMenu();
	void setChessMenu();
	void run();

private:
	sf::RenderWindow* window;
	Menu* menu;
};