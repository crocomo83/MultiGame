#include "../Headers/Replayer.h"
#include "../Headers/Utility.h"
#include "../Headers/ChessBoard.h";
#include "../Headers/Power4Board.h"

Replayer::Replayer(sf::RenderWindow* window_, std::string fileName_)
	: window(window_)
	, fileName(fileName_)
	, board(nullptr)
	, currentPlayer(0)
	, posMouse({ -1, -1 })
	, indexMove(0)
{
	readSavedFile();

	std::vector<std::string> splitStr = splitString(fileName, '_');
	if (splitStr[0] == "Chess") {
		bool reverse = headerLines[0] == "true";
		board = new ChessBoard(reverse);
	}
	else if (splitStr[0] == "Power4") {
		board = new Power4Board();
	}
	else {
		std::cerr << "Game not recognized after reeding file : " << fileName << std::endl;
	}
}

Replayer::~Replayer()
{
}

void Replayer::readSavedFile() {
	saveFile.open("Saves/" + fileName);
	if (!saveFile) {
		std::cerr << "Saves/" << fileName << " not found !" << std::endl;
		return;
	}

	bool moveTagPassed = false;
	std::string line;
	while (std::getline(saveFile, line)) {
		if (line == HEADER_STR) {
			continue;
		}
		else if (line == MOVES_STR) {
			moveTagPassed = true;
			continue;
		}
		else if (moveTagPassed) {
			movesLines.push_back(line);
		}
		else {
			headerLines.push_back(line);
		}
	}
}

void Replayer::run()
{
	while (window->isOpen())
	{
		handleEvent();
		board->update(posMouse, currentPlayer);
		render();
	}
}

void Replayer::handleEvent() {
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		if (event.type == sf::Event::MouseMoved) {
			posMouse.x = event.mouseMove.x;
			posMouse.y = event.mouseMove.y;
		}

		if (event.type == sf::Event::KeyPressed) {
			if (event.key.code == sf::Keyboard::Right) {
				if (indexMove < movesLines.size()) {
					board->play(movesLines[indexMove]);
					indexMove++;
				}
				else {
					std::cout << "Save finished : can't play" << std::endl;
				}
			}
			else if (event.key.code == sf::Keyboard::Left) {
				if (board->undo()) {
					indexMove--;
				}
			}
		}
	}
}

void Replayer::render() {
	window->clear(sf::Color::White);
	board->draw(*window);
	window->display();
}