#pragma once

#include "Menu.h"
#include "IGameState.h"

#include <SFML/Graphics.hpp>
#include <random>
#include <ctime>

class MenuManager : public IGameState{

public:
	MenuManager(sf::RenderWindow* window_);
	~MenuManager();

	// IGameState methods
	void update(sf::Vector2i mousePos) override;
	void render(sf::RenderWindow* window) override;
	int	 handleEvent(sf::Event event) override;

	void setMainMenu();
	std::vector<std::string> listFilesInDirectory(const std::string& folderPath);
	void setReplayList();
	void setChessMenu();
	void setChessMenuVSComputer();
	void setPower4Menu();
	void setPower4MenuVSComputer();

private:
	std::mt19937 rng;
	std::uniform_int_distribution<int> dist;
	sf::RenderWindow* window;
	Menu* menu;
};