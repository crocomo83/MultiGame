#pragma once

#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <stack>

#include "Piece.h"
#include "Utility.h"
#include "IBoard.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Power4Board : public IBoard {

public:
	enum State {
		Normal,
		YellowWin,
		RedWin,
		Equality
	};

public:
									Power4Board();

	void							initBoard();
	void							initializeZobristTable();

	// Interface
	void							draw(sf::RenderWindow& target);
	void							update(sf::Vector2i pos, int idPlayer);

	bool							isGameOver(std::string& messageGameOver);
	void							handleEvent(sf::Vector2i mousePos, sf::Event& event);
	void							selectColumn(int mousePosX);

	// Test de situations
	bool							isColumnFull(int column) const;
	bool							playInColum(int index);
	bool							play(int index);
	bool							undo();

	void							computeValidMoves(int idPlayer);
	void							printValidMoves();

	std::vector<int>				getAllMoves() const;
	int								getNumberMoves();

	std::vector<std::vector<int>>	getAlignments() const;
	std::vector<std::vector<int>>	getAlignmentsLastMove() const;
	std::pair<int, int>				getSimpleCount(std::vector<std::vector<int>> allLines) const;
	std::pair<int, int>				getLinesCount(std::vector<std::vector<int>> allLines, int count) const;
	Power4Board::State				getGameState() const;
	bool							isEquality() const;

	std::pair<bool, float>			getEvaluationEndGame(int level);
	float							getEvaluation();
	float							eval() const;

	std::string						getMoveSymbol(int index);

	uint64_t						hashBoard() const;
	void							printBoard() const;

private:
	sf::RectangleShape rectangle;
	int pieceOnBoard[7][6];
	sf::CircleShape colorCircles[7][6];
	int selectedColumn;

	std::vector<int> history;

	std::unordered_map<uint64_t, float> zobristCache;
	uint64_t zobristTable[42][3];
};
