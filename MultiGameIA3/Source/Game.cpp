#include "../Headers/Game.h"
#include "../Headers/Board.h"
#include "../Headers/Utility.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <iomanip>

Game::Game()
: xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, debug(true)
, selectedPiece(nullptr)
, currentMoveOnMouse(-1)
{
	Player::init();

	window = new sf::RenderWindow();
	window->create(sf::VideoMode(800, 700), "Chess master", sf::Style::Titlebar | sf::Style::Close);
	window->setPosition(sf::Vector2i(0, 0));

	board = new Board();
	players[0].type = Player::PlayerType::Human;
	players[0].id = 0;
	players[1].type = Player::PlayerType::Human;
	players[1].id = 1;

	initFont();
	initDebug();
}

Game::Game(Player::PlayerType whitePlayer, Player::PlayerType blackPlayer)
: xMouse(-1)
, yMouse(-1)
, currentPlayer(0)
, hasSwap(true)
, debug(false)
, selectedPiece(nullptr)
, currentMoveOnMouse(-1)
{
	Player::init();

	window = new sf::RenderWindow();
	window->create(sf::VideoMode(500, 500), "Chess master", sf::Style::Titlebar | sf::Style::Close);
	window->setPosition(sf::Vector2i(0, 0));

	board = new Board();
	players[0].type = whitePlayer;
	players[0].id = 0;
	players[1].type = blackPlayer;
	players[1].id = 1;
}

void Game::initDebug() {
	moveTexts.clear();
	for (int i = 0; i < selectedMove.size() + 1; i++) {
		std::vector <sf::Text> layerText;
		for (int j = 0; j < 100; j++) {
			sf::Text moveText;
			moveText.setFont(*font);
			moveText.setString("");
			moveText.setCharacterSize(14);
			moveText.setOutlineColor(sf::Color::White);
			moveText.setPosition(sf::Vector2f(510 + i * 80, j * 15));
			if (i != selectedMove.size() && j == selectedMove[i]) {
				moveText.setOutlineThickness(1);
			}

			layerText.push_back(moveText);
		}
		moveTexts.push_back(layerText);
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
	board->update(xMouse, yMouse, currentPlayer);
	render();
	while (window->isOpen())
	{
		if (hasSwap) {
			hasSwap = false;
			checkGameOver();
			board->computeValidMoves(currentPlayer);
			if (debug) {
				selectedMove.clear();
				initDebug();
				computeDebug();
				updateDebug();
			}
		}
		handleEvent();
		board->update(xMouse, yMouse, currentPlayer);
		render();
	}
}

void Game::computeDebug() {
	Node::Mode mode = (currentPlayer == 0 ? Node::Mode::Min : Node::Mode::Max);
	lastIAPlay = Player::pool.createNode(0.0f, mode);
	Player::playMinMax(board, 3, currentPlayer, lastIAPlay);

	lastIAPlay->sortChildren();
}

void Game::updateDebug() {
	int size = selectedMove.size();
	for (int i = -1; i < size; i++) {
		Node* toDisplay = lastIAPlay;
		for (int j = 0; j < i + 1; j++) {
			toDisplay = toDisplay->children[selectedMove[j]];
		}
		
		std::vector<Node*>::iterator it;
		int index = 0;
		for (it = toDisplay->children.begin(); it != toDisplay->children.end(); ++it) {
			Node* node = *it;
			moveTexts[i+1][index].setString(node->moveSymbol + " : " + floatToStringWithDecimal(node->value, 1));
			index++;
		}
		for (int j = index; j < 100; j++) {
			moveTexts[i+1][j].setString("");
		}
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
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		if (debug) {
			sf::Vector2i pos_mouse = sf::Mouse::getPosition(*window);
			sf::Vector2f mouse_coord = window->mapPixelToCoords(pos_mouse);

			std::vector<sf::Text> lastMoves = moveTexts[moveTexts.size() - 1];
			currentMoveOnMouse = -1;
			for (int i = 0; i < lastMoves.size(); i++) {
				std::string str = lastMoves[i].getString();
				if (lastMoves[i].getGlobalBounds().contains(mouse_coord)) {
					moveTexts[moveTexts.size() - 1][i].setOutlineThickness(1);
					currentMoveOnMouse = i;
				}
				else {
					moveTexts[moveTexts.size() - 1][i].setOutlineThickness(0);
				}
				
			}

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
				if (currentMoveOnMouse != -1) {
					selectedMove.push_back(currentMoveOnMouse);
					currentMoveOnMouse = -1;
					initDebug();
					updateDebug();
				}
			}

		}

		if (Player::isHuman(players[player].type)) {
			handleActionEvent(event);
		}
	}

	if(!Player::isHuman(players[player].type)){
		computePlay();
	}
}

void Game::computePlay() {
	Move move = Player::play(board, players[currentPlayer].type, players[currentPlayer].id);
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
	window->clear();
	board->draw(*window);
	for (int i = 0; i < moveTexts.size(); i++) {
		for (int j = 0; j < moveTexts[i].size(); j++) {
			window->draw(moveTexts[i][j]);
		}
	}
	window->display();
}