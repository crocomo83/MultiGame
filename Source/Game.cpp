#include "../Headers/Game.h"
#include "../Headers/ChessBoard.h"
#include "../Headers/Power4Board.h"
#include "../Headers/Utility.h"

#include <SFML/Graphics.hpp>

#include <direct.h>
#include <sys/stat.h>
#include <iomanip>
#include <chrono>
#include <fstream> 
#include <iostream>
#include <sstream>
#include <ctime>

Game::Game(GameType gameType_, Player::PlayerType player1, Player::PlayerType player2, const sf::Font* font_, int level)
: gameType(gameType_)
, currentPlayer(0)
, selectedPiece(nullptr)
, currentMoveOnMouse(-1)
, gameOver(false)
, levelIA(level)
, indexDecision(-1)
, gameState(GameState::Playing)
, font(font_)
{
	mousePos = { -1, -1 };
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
	setMenu();

	gameOverText = new sf::Text();
	gameOverText->setFont(*font);
	gameOverText->setString("");
	gameOverText->setCharacterSize(40);
	gameOverText->setOutlineThickness(5);
	gameOverText->setFillColor(sf::Color::Green);
	gameOverText->setPosition({100, 250});
}

Game::~Game() {
	if (saveFile.is_open()) {
		saveFile.close();
	}
}

void Game::update(sf::Vector2i mousePosition) {
	mousePos = mousePosition;

	if (!gameOver) {
		if (!Player::isHuman(players[currentPlayer].type)) {
			indexDecision = Player::play(board, players[currentPlayer].type, players[currentPlayer].id, levelIA);
		}

		// Someone has played
		if (indexDecision != -1) {
			computePlay();
			swapPlayer();
			indexDecision = -1;
		}

		board->update(mousePos);
		checkGameOver();
	}
	
	menu->update(mousePosition);
}

int Game::handleEvent(const sf::Event& event) {
	if (!gameOver && Player::isHuman(players[currentPlayer].type)) {
		indexDecision = board->handleEvent(event);
	}
	menu->handleEvent(event);
	return 0;
}

void Game::render(sf::RenderWindow& window) {
	board->render(window);
	menu->render(window);
	if (gameOver) {
		window.draw(*gameOverText);
	}
}

void Game::checkGameOver() {
	gameOver = isGameOver();
	if (gameOver) {
		std::string messageGameOver = getMessageGameOver();
		std::cout << messageGameOver << std::endl;
		gameOverText->setString(messageGameOver);
		setMenu();
	}
}

std::string Game::getMessageGameOver() const{
	if (gameState == GameState::Player1GiveUp) {
		return board->getPlayerName(1) + " win by forfeit";
	}
	else if (gameState == GameState::Player2GiveUp) {
		return board->getPlayerName(0) + " win by forfeit";
	}
	else {
		BasicBoard::State state = board->getGameState();
		switch (state) {
			case BasicBoard::State::Player1Win:
				return board->getPlayerName(0) + " win !";
			case BasicBoard::State::Player2Win:
				return board->getPlayerName(1) + " win !";
			case BasicBoard::State::Equality:
				return "Equality";
			default:
				return "Error : not game over";
		}
	}
}

bool Game::isGameOver() {
	if (gameState == GameState::Player1GiveUp || gameState == GameState::Player2GiveUp) {
		return true;
	}
	return board->isGameOver();
}

void Game::reset()
{
	gameState = GameState::Playing;
	currentPlayer = 0;
	selectedPiece = nullptr;
	currentMoveOnMouse = -1;
	gameOver = false;
	indexDecision = -1;

	mousePos = { -1, -1 };
	fileName = gameTypeToString(gameType);
	if (gameType == GameType::Chess) {
		bool reverse = !Player::isHuman(players[0].type) && Player::isHuman(players[1].type);
		board = new ChessBoard(reverse);
	}
	else if (gameType == GameType::Power4) {
		board = new Power4Board();
	}

	initSave();
	setMenu();
}

void Game::initSave() {
	// Vérifie si le dossier existe
	struct _stat info;
	if (_stat(SAVES_STR.data(), &info) != 0) {
		// Le dossier n'existe pas, on le crée
		int returnValue = _mkdir(SAVES_STR.data());
	}
	else if (!(info.st_mode & _S_IFDIR)) {
		// Existe mais ce n'est pas un dossier (problème)
		std::cout << "Erreur : '" << SAVES_STR << "' existe mais n'est pas un dossier !" << std::endl;
		return;
	}

	auto t = std::time(nullptr);

	std::tm tm;
	localtime_s(&tm, &t);

	std::ostringstream		oss;
	oss << std::put_time(&tm, "%Y_%m_%d_%H_%M_%S");
	fileName += "_";
	fileName += oss.str();

	saveFile.open(std::string(SAVES_STR) + "/" + fileName);

	writeLine(std::string(HEADER_STR));
	writeLine(board->getHeader());
	writeLine(std::string(MOVES_STR));
	
	std::cout << "save file : " << fileName << std::endl;
}

void Game::setMenu()
{
	menu = new Menu({ 530, 80 }, Layout::Vertical, 60);

	menu->clearOptions();

	if (gameOver) {
		menu->addOption("Replay", [&]() {
			reset();
		});
	}
	else {
		menu->addOption("Give up", [&]() {
			if (currentPlayer == 0) {
				gameState = GameState::Player1GiveUp;
			}
			else {
				gameState = GameState::Player2GiveUp;
			}
		});
	}

	menu->addOption("Main menu", [&]() {
		gameState = GameState::Exit;
	});
	menu->prepareMenu(*font);
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

void Game::computePlay() {
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
}

GameState Game::getGameState() const {
	return gameState;
}