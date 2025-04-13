#include "../Headers/Game.h"
#include "../Headers/ChessBoard.h"
#include "../Headers/Power4Board.h"
#include "../Headers/Utility.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

Game::Game(GameType gameType, Player::PlayerType player1, Player::PlayerType player2)
: xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, selectedPiece(nullptr)
, currentMoveOnMouse(-1)
, gameOver(false)
{
	Player::init();

	window = new sf::RenderWindow();

	if (gameType == GameType::Chess) {
		window->create(sf::VideoMode(540, 540), "Game window", sf::Style::Titlebar | sf::Style::Close);
		window->setPosition(sf::Vector2i(0, 0));

		board = new ChessBoard();
	}
	else if (gameType == GameType::Power4) {
		window->create(sf::VideoMode(700, 700), "Game window", sf::Style::Titlebar | sf::Style::Close);
		window->setPosition(sf::Vector2i(0, 0));

		board = new Power4Board();
	}
	
	players[0].type = player1;
	players[0].id = 0;
	players[1].type = player2;
	players[1].id = 1;
}

void Game::run() {
	while (window->isOpen())
	{
		board->update(xMouse, yMouse, currentPlayer);
		render();

		if (!gameOver && hasSwap) {
			hasSwap = false;
			std::string msgGameOver;
			gameOver = board->isGameOver(msgGameOver);
			if (gameOver) {
				std::cout << msgGameOver << std::endl;
			}
		}

		handleEvent();
	}
}

void Game::handleEvent() {
	int player = board->getCurrentPlayer();
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		if (!gameOver && Player::isHuman(players[currentPlayer].type)) {
			handleActionEvent(event);
		}
	}

	if(!gameOver && !Player::isHuman(players[currentPlayer].type)){
		computePlay();
	}

	currentPlayer = board->getCurrentPlayer();
	if (currentPlayer != player) {
		hasSwap = true;
	}
}

void Game::computePlay() {
	int indexDecision = Player::play(board, players[currentPlayer].type, players[currentPlayer].id);
	std::string moveSymbol = board->getMoveSymbol(indexDecision); // impératif de faire cela avant le play
	if (board->play(indexDecision)) {
		std::cout << (currentPlayer == 0 ? "white" : "black") << " play " << moveSymbol << std::endl;
		std::cout << std::endl;
		swapPlayer();
	}
	else {
		std::cerr << "can't play : " << moveSymbol << std::endl;
		board->printValidMoves();
	}
}

void Game::handleActionEvent(sf::Event &event) {
	if (event.type == sf::Event::MouseMoved) {
		xMouse = event.mouseMove.x;
		yMouse = event.mouseMove.y;
	}
	board->handleEvent(sf::Vector2i(xMouse, yMouse), event);
}

void Game::swapPlayer() {
	currentPlayer = (currentPlayer + 1) % 2;
	hasSwap = true;
}

void Game::render() {
	window->clear(sf::Color::White);
	board->draw(*window);
	window->display();
}