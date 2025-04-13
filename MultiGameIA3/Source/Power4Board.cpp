#include "../Headers/Power4Board.h"

#include <SFML/Window/Event.hpp>

const float startX = 60;
const float startY = 60;
const float offsetX = 70;
const float offsetY = 70;
const float radius = 30;

Power4Board::Power4Board()
	: selectedColumn(-1)
{
	gameOver = false;
	initBoard();
}

void Power4Board::initBoard() {
	rectangle.setSize(sf::Vector2f(500, 440));
	rectangle.setFillColor(sf::Color::Blue);
	rectangle.setPosition(50, 50);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			sf::CircleShape circle(radius);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(startX + x * offsetX, startY + y * offsetY);
			colorCircles[x][y] = circle;
			pieceOnBoard[x][y] = -1;
		}
	}
}

int Power4Board::getNumberMoves() {
	return 0;
}

bool Power4Board::play(int index) {
	if (index < 0 || index > 7) {
		return false;
	}

	for (int y = 5; y >= 0; y--) {
		int value = pieceOnBoard[selectedColumn][y];
		if (value == -1) {
			pieceOnBoard[selectedColumn][y] = idCurrentPlayer;
			break;
		}
	}
	history.push_back(index);
	idCurrentPlayer = otherPlayer(idCurrentPlayer);
	return true;
}

bool Power4Board::undo() {
	history.pop_back();
	return true;
}

void Power4Board::update(int posX, int posY, int idPlayer) {
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			int value = pieceOnBoard[x][y];
			switch (value) {
			case -1:
				colorCircles[x][y].setFillColor(sf::Color::White);
				break;
			case 0:
				colorCircles[x][y].setFillColor(sf::Color::Yellow);
				break;
			case 1:
				colorCircles[x][y].setFillColor(sf::Color::Red);
				break;
			}
		}
	}

	if (selectedColumn != -1) {
		for (int y = 5; y >= 0; y--) {
			int value = pieceOnBoard[selectedColumn][y];
			if (value == -1) {
				colorCircles[selectedColumn][y].setFillColor(sf::Color::Green);
				break;
			}
		}
	}
}

bool Power4Board::isGameOver(std::string& messageGameOver)
{
	if (history.size() == 0) {
		return false;
	}
	int lastColumnPlayed = history[history.size() - 1];
	int lastRowPlayed = -1;

	int count = 0;
	int idPlayer = -1;

	for (int y = 0; y < 6; y++) {
		int value = pieceOnBoard[lastColumnPlayed][y];
		if (value != -1) {
			if (lastRowPlayed == -1) {
				lastRowPlayed = y;
			}
			if (value == idPlayer) {
				count++;
				if (count == 4) {
					std::string namePlayer = (idPlayer == 0 ? "Yellow" : "Red");
					messageGameOver = "Game over : " + namePlayer + " win !";
					selectedColumn = -1;
					return true;
				}
			}
			else {
				count = 1;
				idPlayer = value;
			}
		}
	}

	count = 0;
	idPlayer = -1;
	for (int x = 0; x < 7; x++) {
		int value = pieceOnBoard[x][lastRowPlayed];
		if (value != -1) {
			if (value == idPlayer) {
				count++;
				if (count == 4) {
					std::string namePlayer = (idPlayer == 0 ? "Yellow" : "Red");
					messageGameOver = "Game over : " + namePlayer + " win !";
					selectedColumn = -1;
					return true;
				}
			}
			else {
				count = 1;
				idPlayer = value;
			}
		}
	}

	count = 0;
	idPlayer = -1;
	int constanteDiag = lastColumnPlayed + lastRowPlayed;
	for (int x = 0; x < 7; x++) {
		if ((constanteDiag - x) >= 0 && (constanteDiag - x) <= 5) {
			int value = pieceOnBoard[x][constanteDiag - x];
			if (value != -1) {
				if (value == idPlayer) {
					count++;
					if (count == 4) {
						std::string namePlayer = (idPlayer == 0 ? "Yellow" : "Red");
						messageGameOver = "Game over : " + namePlayer + " win !";
						selectedColumn = -1;
						return true;
					}
				}
				else {
					count = 1;
					idPlayer = value;
				}
			}
		}
	}

	count = 0;
	idPlayer = -1;
	constanteDiag = lastColumnPlayed - lastRowPlayed;
	for (int x = 0; x < 7; x++) {
		if ((x - constanteDiag) >= 0 && (x - constanteDiag) <= 5) {
			int value = pieceOnBoard[x][x - constanteDiag];
			if (value != -1) {
				if (value == idPlayer) {
					count++;
					if (count == 4) {
						std::string namePlayer = (idPlayer == 0 ? "Yellow" : "Red");
						messageGameOver = "Game over : " + namePlayer + " win !";
						selectedColumn = -1;
						return true;
					}
				}
				else {
					count = 1;
					idPlayer = value;
				}
			}
		}
	}

	return false;
}

void Power4Board::handleEvent(sf::Vector2i mousePos, sf::Event& event)
{
	selectColumn(mousePos.x);
	switch (event.type) {
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			play(selectedColumn);
		}
		break;
	}
}

void Power4Board::selectColumn(int mousePosX) {
	int x = mousePosX;
	if (x >= startX) {
		int column = (x - startX) / offsetX;
		if (column >= 0 && column <= 6) {
			selectedColumn = column;
		}
		else {
			selectedColumn = -1;
		}
	}
	else {
		selectedColumn = -1;
	}
}

void Power4Board::draw(sf::RenderWindow& target) {
	target.draw(rectangle);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			target.draw(colorCircles[x][y]);
		}
	}
}

Power4Board::State Power4Board::getGameState() {
	return Power4Board::State::Normal;
}

void Power4Board::computeValidMoves(int idPlayer) {
}

void Power4Board::printValidMoves() {
}

std::pair<bool, float> Power4Board::getEvaluationEndGame() {
	return { true, 0 };
}

float Power4Board::getEvaluation() {
	uint64_t hash = hashBoard();
	auto it = zobristCache.find(hash);
	if (it != zobristCache.end()) {
		return it->second;
	}

	float value = eval();
	zobristCache[hash] = value;
	return value;
}

float Power4Board::eval() {
	float score = 0;
	return score;
}

std::string Power4Board::getMoveSymbol(int index) {
	return "";
}

uint64_t Power4Board::hashBoard() const {
	uint64_t hash = 0;
	return hash;
}