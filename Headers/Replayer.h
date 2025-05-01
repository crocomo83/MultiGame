#pragma once
#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>
#include "IBoard.h"

class Replayer {

public:
						Replayer(sf::RenderWindow* window_, std::string fileName);
						~Replayer();
	void				readSavedFile();
	void				run();
	void				handleEvent();

	void				render();

private:
	sf::RenderWindow*	window;
	std::string			fileName;
	std::ifstream		saveFile;
	IBoard*				board;
	std::vector <std::string> headerLines;
	std::vector <std::string> movesLines;
	int	currentPlayer;
	sf::Vector2i posMouse;
	int indexMove;
};