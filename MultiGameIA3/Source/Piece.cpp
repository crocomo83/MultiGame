#include "../Headers/Piece.h"

#include <iostream>

bool equalMoves(Move move1, Move move2) {
	return move1.begin == move2.begin
		&& move1.end == move2.end
		&& move1.player == move2.player;
}

std::string getSymbolPosX(int x) {
	switch (x) {
	case 0:
		return "a";
	case 1:
		return "b";
	case 2:
		return "c";
	case 3:
		return "d";
	case 4:
		return "e";
	case 5:
		return "f";
	case 6:
		return "g";
	case 7:
		return "h";
	default:
		return "ERROR";
	}
}

std::string getSymbolPosY(int y) {
	return std::to_string(y + 1);
}

std::string getSymbolPosition(sf::Vector2i pos) {
	return getSymbolPosX(pos.x) + getSymbolPosY(pos.y);
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

std::string tagToStr(Tag tag) {
	switch (tag) {
	case NoneTag:
		return "NoneTag";
	case EmptyMove:
		return "EmptyMove";
	case JumpTwoCases:
		return "JumpTwoCases";
	case EnPassant:
		return "EnPassant";
	case Promotion:
		return "Promotion";
	case KingSideCastling:
		return "KingSideCastling";
	case QueenSideCastling:
		return "QueenSideCastling";
	}
}

std::string pieceToStr(Piece* piece) {
	if (piece == nullptr) {
		return " ";
	}
	else if (piece->player == 0) {
		switch (piece->type) {
		case PieceType::King:
			return "R";
			break;
		case PieceType::Queen:
			return "D";
			break;
		case PieceType::Bishop:
			return "F";
			break;
		case PieceType::Knight:
			return "C";
			break;
		case PieceType::Tower:
			return "T";
			break;
		case PieceType::Pawn:
			return "P";
			break;

		}
	}
	else {
		switch (piece->type) {
		case PieceType::King:
			return "r";
			break;
		case PieceType::Queen:
			return "d";
			break;
		case PieceType::Bishop:
			return "f";
			break;
		case PieceType::Knight:
			return "c";
			break;
		case PieceType::Tower:
			return "t";
			break;
		case PieceType::Pawn:
			return "p";
			break;
		}
	}
}

