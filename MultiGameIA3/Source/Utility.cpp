#include "../Headers/Utility.h"
#include <sstream>
#include <iomanip>
#include <cmath>

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