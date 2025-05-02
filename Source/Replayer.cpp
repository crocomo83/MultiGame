#include "../Headers/Replayer.h"
#include "../Headers/Utility.h"
#include "../Headers/ChessBoard.h";
#include "../Headers/Power4Board.h"

Replayer::Replayer(std::string fileName_)
	: fileName(fileName_)
	, board(nullptr)
	, currentPlayer(0)
	, indexMove(0)
{
	readSavedFile();

	if (headerLines[0] == "Chess") {
		bool reverse = headerLines[1] == "1";
		board = new ChessBoard(reverse);
	}
	else if (headerLines[0] == "Power4") {
		board = new Power4Board();
	}
	else {
		std::cerr << "Game not recognized after reeding file : " << fileName << std::endl;
	}
}

Replayer::~Replayer()
{
	if (saveFile.is_open()) {
		saveFile.close();
	}
}

void Replayer::readSavedFile() {
	saveFile.open(std::string(SAVES_STR) + "/" + fileName);
	if (!saveFile) {
		std::cerr << SAVES_STR << "/" << fileName << " not found !" << std::endl;
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

void Replayer::update(sf::Vector2i mousePosition)
{
	board->update(mousePosition);
}

int Replayer::handleEvent(const sf::Event& event) {
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
	return 0;
}

void Replayer::render(sf::RenderWindow& window) {
	board->render(window);
}