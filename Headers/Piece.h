#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <string>

enum PieceType {
	King,
	Queen,
	Bishop,
	Knight,
	Tower,
	Pawn, 
	None
};

enum Tag {
	NoneTag,
	JumpTwoCases,
	EnPassant,
	Promotion,
	KingSideCastling,
	QueenSideCastling,
	EmptyMove
};

struct Piece {
	PieceType type;
	int player;
	sf::Vector2i pos;
	int nbMove;
	bool hasJustMoveTwoCases;
	bool taken;

	Piece(PieceType type_, int player_, sf::Vector2i pos_)
		: type(type_)
		, player(player_)
		, pos(pos_)
		, nbMove(0)
		, hasJustMoveTwoCases(false)
		, taken(false)
	{}
};

struct Move {
	sf::Vector2i begin;
	sf::Vector2i end;
	Piece* piece;
	int player;
	Piece* destroyed;
	Tag tag;

	Move()
		: begin(sf::Vector2i(-1, -1))
		, end(sf::Vector2i(-1, -1))
		, piece(nullptr)
		, player(-1)
		, destroyed(nullptr)
		, tag(EmptyMove)
	{}

	Move(Piece* piece_, sf::Vector2i begin_, sf::Vector2i end_)
		: begin(begin_)
		, end(end_)
		, piece(piece_)
		, player(piece_->player)
		, destroyed(nullptr)
		, tag(NoneTag)
	{}

	Move(Piece* piece_, sf::Vector2i end_)
		: Move(piece_, piece_->pos, end_)
	{}
};

bool equalMoves(Move move1, Move move2);
std::string getSymbolPosX(int x);
std::string getSymbolPosY(int y);
std::string getSymbolPosition(sf::Vector2i pos);
std::string getId(Piece* piece);
int getType(Piece* piece);
std::string tagToStr(Tag tag);
std::string pieceToStr(Piece* piece);