#include "../Headers/Game.h"
#include "../Headers/ChessBoard.h"
#include "../Headers/Power4Board.h"
#include "../Headers/Utility.h"

#include <SFML/Graphics.hpp>

#include <direct.h>
#include <sys/stat.h>
#include <iomanip>
#include <chrono>
#include <format>
#include <fstream> 
#include <iostream>
#include <sstream>
#include <ctime>

Game::Game(sf::RenderWindow* window_, GameType gameType, Player::PlayerType player1, Player::PlayerType player2, int level)
: window(window_)
, posMouse({-1, -1})
, currentPlayer(0)
, hasSwap(true)
, selectedPiece(nullptr)
, currentMoveOnMouse(-1)
, gameOver(false)
, levelIA(level)
{
	fileName = gameTypeToString(gameType);
	if (gameType == GameType::Chess) {
		bool reverse = !Player::isHuman(player1) && Player::isHuman(player2);
		board = new ChessBoard(reverse);
	}
	else if (gameType == GameType::Power4) {
		board = new Power4Board();
	}
	
	players[0].type = player1;
	players[0].id = 0;
	players[1].type = player2;
	players[1].id = 1;

	initSave();
}

Game::~Game(){
	if (saveFile.is_open()) {
		saveFile.close();
	}
}

void Game::initSave() {
	// Vérifie si le dossier existe
	struct _stat info;
	if (_stat("Saves", &info) != 0) {
		// Le dossier n'existe pas, on le crée
		_mkdir("Saves");
	}
	else if (!(info.st_mode & _S_IFDIR)) {
		// Existe mais ce n'est pas un dossier (problème)
		std::cout << "Erreur : 'Saves' existe mais n'est pas un dossier !" << std::endl;
		return;
	}

	auto t = std::time(nullptr);

	std::tm tm;
	localtime_s(&tm, &t);

	std::ostringstream		oss;
	oss << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S");
	fileName += "_";
	fileName += oss.str();

	saveFile.open("Saves/" + fileName);

	writeLine("HEADER");
	writeLine(board->getHeader());
	writeLine("MOVES");
	
	std::cout << "save file : " << fileName << std::endl;
}

std::string Game::gameTypeToString(GameType gameType)
{
	switch (gameType) {
	case GameType::Chess:
		return "Chess";
	case GameType::Power4:
		return "Power4";
	default:
		return "Not found";
	}
}

void Game::writeLine(const std::string& texte) {
	if (saveFile.is_open()) {
		saveFile << texte << std::endl;
	}
}

void Game::run() {
	int indexDecision = -1;
	while (window->isOpen())
	{
		board->update(posMouse, currentPlayer);
		render();

		if (!gameOver && hasSwap) {
			hasSwap = false;
			std::string msgGameOver;
			gameOver = board->isGameOver(msgGameOver);
			if (gameOver) {
				std::cout << msgGameOver << std::endl;
			}
		}

		indexDecision = handleEvent();

		if (!gameOver && !Player::isHuman(players[currentPlayer].type)) {
			indexDecision = Player::play(board, players[currentPlayer].type, players[currentPlayer].id, levelIA);
		}

		// Someone has played
		if (indexDecision != -1) {
			computePlay(indexDecision);
			swapPlayer();
			indexDecision = -1;
		}
	}
}

int Game::handleEvent() {
	int indexDecision = -1;
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		if (!gameOver && Player::isHuman(players[currentPlayer].type)) {
			if (event.type == sf::Event::MouseMoved) {
				posMouse.x = event.mouseMove.x;
				posMouse.y = event.mouseMove.y;
			}
			indexDecision = board->handleEvent(posMouse, event);
		}
	}
	return indexDecision;
}

void Game::computePlay(int indexDecision) {
	std::string strToWright = board->getStringToWright(indexDecision);
	std::string moveSymbol = board->getMoveSymbol(indexDecision); // impératif de faire cela avant le play
	if (indexDecision == -1) {
		std::cerr << "Decision not made : can't play" << std::endl;
	}
	else if (board->play(indexDecision)) {
		writeLine(strToWright);
		std::cout << (currentPlayer == 0 ? "white" : "black") << " play " << moveSymbol << std::endl;
		std::cout << std::endl;
	}
	else {
		std::cerr << "can't play : " << moveSymbol << std::endl;
	}
}

void Game::swapPlayer() {
	currentPlayer = otherPlayer(currentPlayer);
	hasSwap = true;
}

void Game::render() {
	window->clear(sf::Color::White);
	board->draw(*window);
	window->display();
}