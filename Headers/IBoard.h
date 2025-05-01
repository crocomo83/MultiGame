#pragma once

#include <SFML\Graphics\RenderWindow.hpp>
#include <unordered_map>

class IBoard {

public:
	virtual							~IBoard() = default;

	// Interface
	virtual void					draw(sf::RenderWindow& target) = 0;
	virtual void					update(sf::Vector2i pos, int idPlayer) = 0;

	virtual bool					isGameOver(std::string& message) = 0;
	virtual int						handleEvent(sf::Vector2i mousPos, sf::Event& event) = 0;

	virtual std::string				getStringToWright(int indexMove) const = 0;
	virtual std::string				getHeader() const = 0;

	virtual void					computeValidMoves(int idPlayer) = 0;
	virtual void					printValidMoves() = 0;
	virtual bool					play(int index) = 0;
	virtual bool					play(std::string moveStr) = 0;
	virtual bool					undo() = 0;

	virtual int						getNumberMoves() = 0;

	virtual std::pair<bool, float>	getEvaluationEndGame(int level) = 0;
	virtual float					getEvaluation() = 0;

	virtual std::string				getMoveSymbol(int index) = 0;

	virtual uint64_t				hashBoard() const = 0;
	virtual void					printBoard() const = 0;

	virtual int	getCurrentPlayer() {
		return idCurrentPlayer;
	}

protected:
	bool								gameOver;
	sf::Vector2i						mousePos;
	int									idCurrentPlayer;
	std::unordered_map<uint64_t, float> zobristCache;
};
