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

std::string floatToStringWithDecimal(float value, int decimal) {
	std::ostringstream stream;
	stream << std::fixed << std::setprecision(decimal) << value;
	return stream.str();
}