#include "../Headers/Utility.h"

int otherPlayer(int idPlayer) {
	return (idPlayer + 1) % 2;
}

bool isOnBoard(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}