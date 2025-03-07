#include "../Headers/Game.h"
#include "../Headers/Board.h"

#include <SFML/Graphics.hpp>
#include <iostream>

Game::Game() 
: window(sf::VideoMode(700, 600), "Chess master")
, xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, debug(true)
, selectedPiece(nullptr)
{
	board = new Board();
	players[0] = new Player(Player::PlayerType::Human, 0);
	players[1] = new Player(Player::PlayerType::Human, 1);
}

Game::Game(Player::PlayerType whitePlayer, Player::PlayerType blackPlayer)
: window(sf::VideoMode(500, 500), "Chess master")
, xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, debug(false)
{
	board = new Board();
	players[0] = new Player(whitePlayer, 0);
	players[1] = new Player(blackPlayer, 1);
}

void Game::run() {
	
	while (window.isOpen())
	{
		if (hasSwap) {
			hasSwap = false;
			checkGameOver();
			board->computeValidMoves(currentPlayer);
		}
		handleEvent();
		board->update(xMouse, yMouse, currentPlayer);
		if (debug) {
			board->updateDebug();
		}
		render();
	}
}

void Game::checkGameOver() {
	if (board->isCheckMate(0)) {
		std::cout << "Check Mate, White win!" << std::endl;
	}
	else if (board->isCheckMate(1)) {
		std::cout << "Check Mate, Black win!" << std::endl;
	}
	else if (board->isEquality()) {
		std::cout << "Equality" << std::endl;
	}
	else {
		return;
	}
	std::cout << "Game over" << std::endl;
	while (true) {

	}
}

void Game::handleEvent() {
	int player = currentPlayer;
	sf::Event event;
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window.close();
		}

		if (players[player]->isHuman()) {
			handleActionEvent(event);
		}
	}

	if(!players[player]->isHuman()){
		computePlay();
	}
}

void Game::computePlay() {
	Move move = players[currentPlayer]->play(board);
	if (board->play(move, true)) {
		std::cout << (currentPlayer == 0 ? "white" : "black") << " play " << board->getMoveSymbol(move) << std::endl;
		std::cout << std::endl;
		swapPlayer();
	}
	else {
		std::cerr << "can't play : " << board->getMoveSymbol(move) << std::endl;
		board->printValidMoves();
	}
}

void Game::handleActionEvent(sf::Event &event) {
	switch (event.type) {
		case sf::Event::MouseMoved:
			xMouse = event.mouseMove.x;
			yMouse = event.mouseMove.y;
			break;
		case sf::Event::MouseButtonPressed:
			if (event.mouseButton.button == sf::Mouse::Left) {
				selectedPiece = board->select(xMouse, yMouse, currentPlayer);
			}
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				if (board->unselect(xMouse, yMouse, selectedPiece)) {
					swapPlayer();
				}
			}
			break;
	}
}

void Game::swapPlayer() {
	currentPlayer = (currentPlayer + 1) % 2;
	hasSwap = true;
}

void Game::render() {
	window.clear();
	board->draw(window);
	window.display();
}