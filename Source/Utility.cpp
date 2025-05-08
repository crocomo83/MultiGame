#include "../Headers/Utility.h"
#include <sstream>
#include <iomanip>
#include <cmath>
#include <vector>

std::vector<std::string> splitString(const std::string& str, char delimiter) {
	std::vector<std::string> tokens;
	std::stringstream ss(str);
	std::string token;

	while (std::getline(ss, token, delimiter)) {
		tokens.push_back(token);
	}

	return tokens;
}

int otherPlayer(int idPlayer) {
	return (idPlayer + 1) % 2;
}

bool isOnBoard(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool isOnBoard(sf::Vector2i pos) {
	return pos.x >= 0 && pos.x < 8 && pos.y >= 0 && pos.y < 8;
}

sf::Vector2i* getKnightEmplacement(sf::Vector2i pos) {
	sf::Vector2i possibilities[8];
	int x = pos.x;
	int y = pos.y;

	possibilities[0] = { x - 2, y - 1 };
	possibilities[1] = { x - 2, y + 1 };
	possibilities[2] = { x + 2, y - 1 };
	possibilities[3] = { x + 2, y + 1 };
	possibilities[4] = { x - 1, y - 2 };
	possibilities[5] = { x - 1, y + 2 };
	possibilities[6] = { x + 1, y - 2 };
	possibilities[7] = { x + 1, y + 2 };

	return possibilities;
}

std::string floatToStringWithDecimal(float value, int decimal) {
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(decimal) << value;
	return stream.str();
}

sf::Vector2i PixelToChessBoard(sf::Vector2i posPixel, sf::Vector2i start, sf::Vector2i offset, bool reverse) {
	int posX, posY;
	if (reverse) {
		posX = 7 - (int)((float)(posPixel.x - start.x) / offset.x);
		posY = (int)((float)(posPixel.y - start.y) / offset.y);
	}
	else {
		posX = (int)((float)(posPixel.x - start.x) / offset.x);
		posY =  7 - (int)((float)(posPixel.y - start.y) / offset.y);
	}
	return sf::Vector2i(posX, posY);
}

sf::Vector2i CoordToPixelChessBoard(sf::Vector2i posBoard, sf::Vector2i start, sf::Vector2i offset, bool reverse) {
	int posX, posY;
	if (reverse) {
		posX = start.x + (7 - posBoard.x) * offset.x;
		posY = start.y + posBoard.y * offset.y;
	}
	else {
		posX = start.x + posBoard.x * offset.x;
		posY = start.y + (7 - posBoard.y) * offset.y;
	}
	return sf::Vector2i(posX, posY);
}
