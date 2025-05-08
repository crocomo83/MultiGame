#pragma once

#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <stack>

#include "Piece.h"
#include "Utility.h"
#include "BasicBoard.h"
#include "Graphics/IDrawable.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>

class Power4Board : public BasicBoard {

public:
							Power4Board();

	// ===== IDrawable methods =====
	void					update(sf::Vector2i mousePosition) override;
	void					render(sf::RenderWindow& window) override;
	int						handleEvent(const sf::Event& event) override;
	void					reset() override;

	// ===== BasicBoard methods =====
	bool					isGameOver() override;
	
	void					printValidMoves() override;
	bool					play(int index) override;
	bool					play(std::string moveStr) override;
	bool					undo() override;

	int						getNumberMoves() override;
	std::string				getStringToWright(int indexMove) const override;
	std::string				getHeader() const override;
	std::pair<bool, float>	getEvaluationEndGame(int level) override;
	float					getEvaluation() override;
	std::string				getMoveSymbol(int index) override;
	std::string				getPlayerName(int player) const override;
	BasicBoard::State		getGameState() const override;

	uint64_t				hashBoard() const override;
	void					printBoard() const override;

	// ===== Power4Board methods =====
	void							initBoard();
	void							initializeZobristTable();

	void							selectColumn(int mousePosX);

	// Test de situations
	bool							isColumnFull(int column) const;
	bool							playInColum(int index);

	std::vector<int>				getAllMoves() const;
	std::vector<std::vector<int>>	getAlignments() const;
	std::vector<std::vector<int>>	getAlignmentsLastMove() const;
	std::pair<int, int>				getSimpleCount(std::vector<std::vector<int>> allLines) const;
	std::pair<int, int>				getLinesCount(std::vector<std::vector<int>> allLines, int count) const;

	bool							isEquality() const;

	float							eval() const;

private:
	void							computeGameState();

private:
	sf::RectangleShape rectangle;
	int pieceOnBoard[7][6];
	sf::CircleShape colorCircles[7][6];
	int selectedColumn;

	std::vector<int> history;
	std::vector<State> historyState;

	std::unordered_map<uint64_t, float> zobristCache;
	uint64_t zobristTable[42][3];
};
