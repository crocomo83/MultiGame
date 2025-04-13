#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <unordered_map>

class IBoard {

public:
	virtual							~IBoard() = default;

	// Interface
	virtual void					draw(sf::RenderWindow& target) = 0;
	virtual void					update(int x, int y, int idPlayer) = 0;

	virtual bool					isGameOver(std::string& message) = 0;
	virtual void					handleEvent(sf::Vector2i mousPos, sf::Event& event) = 0;

	virtual void					computeValidMoves(int idPlayer) = 0;
	virtual void					printValidMoves() = 0;
	virtual bool					play(int index) = 0;
	virtual bool					undo() = 0;

	virtual int						getNumberMoves() = 0;

	virtual std::pair<bool, float>	getEvaluationEndGame() = 0;
	virtual float					getEvaluation() = 0;

	virtual std::string				getMoveSymbol(int index) = 0;

	virtual uint64_t				hashBoard() const = 0;

	virtual int	getCurrentPlayer() {
		return idCurrentPlayer;
	}

protected:
	bool								gameOver;
	sf::Vector2f						mousePos;
	int									idCurrentPlayer;
	std::unordered_map<uint64_t, float> zobristCache;
};
