#pragma once

#include "Graphics/IDrawable.h"

#include <SFML\Graphics\RenderWindow.hpp>
#include <unordered_map>

class BasicBoard : public IDrawable {

public:
	enum State {
		Playing,
		Player1Win,
		Player2Win,
		Equality
	};

public:
	
									BasicBoard();
	virtual							~BasicBoard() = default;

	// ===== Class methods =====
	int								getCurrentPlayer() const;

	// ===== Virtual methods =====
	virtual bool					isGameOver() = 0;

	virtual void					printValidMoves() = 0;
	virtual bool					play(int index) = 0;
	virtual bool					play(std::string moveStr) = 0;
	virtual bool					undo() = 0;

	virtual std::string				getStringToWright(int indexMove) const = 0;
	virtual std::string				getHeader() const = 0;
	virtual int						getNumberMoves() = 0;
	virtual std::pair<bool, float>	getEvaluationEndGame(int level) = 0;
	virtual float					getEvaluation() = 0;
	virtual std::string				getMoveSymbol(int index) = 0;
	virtual std::string				getPlayerName(int player) const = 0;

	virtual State					getGameState() const = 0;
	virtual void					computeGameState() = 0;

	virtual uint64_t				hashBoard() const = 0;
	virtual void					printBoard() const = 0;

protected:
	bool								gameOver;
	sf::Vector2i						mousePos;
	int									idCurrentPlayer;
	std::unordered_map<uint64_t, float> zobristCache;
};
