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

	void					initBoard();

	// Interface
	void					draw(sf::RenderWindow& target);
	void					update(int posX, int posY, int idPlayer);

	bool					isGameOver(std::string& messageGameOver);
	void					handleEvent(sf::Vector2i mousePos, sf::Event& event);
	void					selectColumn(int mousePosX);

	// Test de situations
	bool					play(int index);
	bool					undo();

	void					computeValidMoves(int idPlayer);
	void					printValidMoves();

	int						getNumberMoves();

	Power4Board::State		getGameState();

	std::pair<bool, float>	getEvaluationEndGame();
	float					getEvaluation();
	float					eval();

	std::string				getMoveSymbol(int index);

	uint64_t				hashBoard() const;

private:
	sf::RectangleShape rectangle;
	int pieceOnBoard[7][6];
	sf::CircleShape colorCircles[7][6];
	int selectedColumn;

	std::vector<int> history;

	std::unordered_map<uint64_t, float> zobristCache;
	uint64_t zobristTable[64][12];
};
