#include "../Headers/Game.h"
#include "../Headers/Board.h"
#include "../Headers/Utility.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

Game::Game() 
: window(sf::VideoMode(700, 700), "Chess master")
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

	initFont();
	initDebug();

	for (int i = 0; i < 100; i++) {
		sf::Text moveText;
		moveText.setFont(*font);
		moveText.setString("");
		moveText.setCharacterSize(12);
		moveText.setOutlineColor(sf::Color::White);
		moveText.setPosition(sf::Vector2f(510, i * 10));

		moveTexts.push_back(moveText);
	}
}

Game::Game(Player::PlayerType whitePlayer, Player::PlayerType blackPlayer)
: window(sf::VideoMode(500, 500), "Chess master")
, xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, debug(false)
, selectedPiece(nullptr)
{
	board = new Board();
	players[0] = new Player(whitePlayer, 0);
	players[1] = new Player(blackPlayer, 1);
}

void Game::initDebug() {
	for (int i = 0; i < 100; i++) {
		sf::Text moveText;
		moveText.setFont(*font);
		moveText.setString("");
		moveText.setCharacterSize(14);
		moveText.setOutlineColor(sf::Color::White);
		moveText.setPosition(sf::Vector2f(510, i * 14));

		moveTexts.push_back(moveText);
	}
}

void Game::initFont() {
	font = new sf::Font();
	if (!font->loadFromFile("arial.ttf"))
	{
		// erreur...
	}
}

void Game::run() {
	
	while (window.isOpen())
	{
		if (hasSwap) {
			hasSwap = false;
			checkGameOver();
			board->computeValidMoves(currentPlayer);
			if (debug) {
				updateDebug();
			}
		}
		handleEvent();
		board->update(xMouse, yMouse, currentPlayer);
		if (debug) {
			board->updateDebug();
		}
		render();
	}
}

void Game::updateDebug() {
	std::map<std::string, float> valueMoves = players[currentPlayer]->getMinMaxAllMovesValue(board, 3, currentPlayer);

	// Copier les éléments dans un std::vector de paires
	std::vector<std::pair<std::string, float>> vec(valueMoves.begin(), valueMoves.end());

	// Trier par valeur (float)
	if (currentPlayer == 0) {
		std::sort(vec.begin(), vec.end(),
			[](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
				return a.second < b.second;
			});
	}
	else {
		std::sort(vec.begin(), vec.end(),
			[](const std::pair<std::string, float>& a, const std::pair<std::string, float>& b) {
				return a.second > b.second;
			});
	}

	std::vector<std::pair<std::string, float>>::iterator it;
	int index = 0;
	for (it = vec.begin(); it != vec.end(); ++it) {
		std::pair<std::string, float> valueMove = *it;
		moveTexts[index].setString(valueMove.first + " : " + floatToStringWithDecimal(valueMove.second, 1));
		index++;
	}
	for (int i = index; i < 100; i++) {
		moveTexts[i].setString("");
	}
}

void Game::checkGameOver() {
	Board::State state = board->getGameState();
	switch (state) {
		case Board::State::CheckMateWhite :
			std::cout << "Check Mate, White win!" << std::endl;
			break;
		case Board::State::CheckMateBlack:
			std::cout << "Check Mate, Black win!" << std::endl;
			break;
		case Board::State::Equality:
			std::cout << "Equality" << std::endl;
			break;
		default:
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
	for (int i = 0; i < moveTexts.size(); i++) {
		window.draw(moveTexts[i]);
	}
	window.display();
}