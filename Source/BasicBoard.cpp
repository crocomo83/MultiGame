#include "../Headers/BasicBoard.h"

BasicBoard::BasicBoard()
: gameOver(false)
, idCurrentPlayer(0)
{
	mousePos = sf::Vector2i(-1, -1);
}

int	BasicBoard::getCurrentPlayer() const {
	return idCurrentPlayer;
}
