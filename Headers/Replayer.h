#pragma once

#include "BasicBoard.h"
#include "Graphics/IDrawable.h"
#include "Graphics/Menu.h"

#include <string>
#include <fstream>
#include <SFML/Graphics.hpp>


class Replayer : public IDrawable {

public:
						Replayer(std::string fileName);
						~Replayer();

	void				readSavedFile();

	// IDrawable methods
	void				update(sf::Vector2i mousePosition) override;
	void				render(sf::RenderWindow& window) override;
	int					handleEvent(const sf::Event& event) override;
	void				reset() override;

private:
	std::string			fileName;
	std::ifstream		saveFile;
	BasicBoard*				board;
	std::vector <std::string> headerLines;
	std::vector <std::string> movesLines;
	int	currentPlayer;
	int indexMove;
};