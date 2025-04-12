#include "../Headers/Piece.h"

#include <iostream>

bool equalMoves(Move move1, Move move2) {
	return move1.begin == move2.begin
		&& move1.end == move2.end
		&& move1.player == move2.player;
}

std::string getSymbolPosition(sf::Vector2i pos) {
	std::string posY = std::to_string(pos.y + 1);
	switch (pos.x) {
	case 0:
		return "a" + posY;
	case 1:
		return "b" + posY;
	case 2:
		return "c" + posY;
	case 3:
		return "d" + posY;
	case 4:
		return "e" + posY;
	case 5:
		return "f" + posY;
	case 6:
		return "g" + posY;
	case 7:
		return "h" + posY;
	}
	return "ERROR";
}

std::string getId(Piece* piece) {
	switch (piece->type) {
	case PieceType::King:
		return "R";
	case PieceType::Queen:
		return "D";
	case PieceType::Bishop:
		return "F";
	case PieceType::Knight:
		return "C";
	case PieceType::Tower:
		return "T";
	case PieceType::Pawn:
		return "";
	}
	return "error";
}

int getType(Piece* piece) {
	if (piece->player == 0) {
		switch (piece->type) {
		case PieceType::King:
			return 1;
		case PieceType::Queen:
			return 2;
		case PieceType::Bishop:
			return 3;
		case PieceType::Knight:
			return 4;
		case PieceType::Tower:
			return 5;
		case PieceType::Pawn:
			return 6;
		}
	}
	else {
		switch (piece->type) {
		case PieceType::King:
			return 7;
		case PieceType::Queen:
			return 8;
		case PieceType::Bishop:
			return 9;
		case PieceType::Knight:
			return 10;
		case PieceType::Tower:
			return 11;
		case PieceType::Pawn:
			return 12;
		}
	}
	
}