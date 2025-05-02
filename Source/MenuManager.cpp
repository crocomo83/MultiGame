#include "../Headers/MenuManager.h"
#include "../Headers/Game.h"
#include "../Headers/Replayer.h"

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <thread>

namespace fs = std::filesystem;

const int chessLevelIA = 4;
const int power4LevelIA = 6;

const float spaceBetweenTextMenu = 80;
const float spaceBetweenTextList = 30;

MenuManager::MenuManager(sf::RenderWindow* window_)
	: window(window_)
	, rng(static_cast<unsigned int>(std::time(nullptr)))
	, dist(0, 1)
{
	menu = new Menu(window, spaceBetweenTextMenu);
	setMainMenu();
}

MenuManager::~MenuManager() {
}

void MenuManager::setMainMenu() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

	menu->addOption("Chess", [&]() {
		setChessMenu();
		});

	menu->addOption("Power 4", [&]() {
		setPower4Menu();
		});

	menu->addOption("Replay", [&]() {
		setReplayList();
		});

	menu->addOption("Quit", [&]() {
		window->close();
		});
}

std::vector<std::string> MenuManager::listFilesInDirectory(const std::string& folderPath) {
	std::vector<std::string> fileNames;

	try {
		for (const auto& entry : fs::directory_iterator(folderPath)) {
			if (entry.is_regular_file()) {
				fileNames.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const fs::filesystem_error& e) {
		std::cerr << "Erreur lors de l'accès au dossier : " << e.what() << std::endl;
	}

	return fileNames;
}

void MenuManager::setReplayList() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextList);

	std::vector<std::string> fileNames = listFilesInDirectory(std::string(SAVES_STR));
	for (std::string fileName : fileNames) {
		menu->addOption(fileName, [fileName, this]() {
			Replayer replayer(fileName);
			replayer.run();
			});
	}

	menu->addOption("Main Menu", [&]() {
		setMainMenu();
		});
}

void MenuManager::setChessMenu() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

	menu->addOption("Player vs Computer", [&]() {
		setChessMenuVSComputer();
		});

	menu->addOption("Player vs Player", [&]() {
		Game game(window, Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::Human);
		game.run();
		});

	menu->addOption("Main Menu", [&]() {
		setMainMenu();
		});
}

void MenuManager::setChessMenuVSComputer() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

	menu->addOption("Play White", [&]() {
		Game game(window, Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::AlphaBeta, chessLevelIA);
		game.run();
		});

	menu->addOption("Play Black", [&]() {
		Game game(window, Game::GameType::Chess, Player::PlayerType::AlphaBeta, Player::PlayerType::Human, chessLevelIA);
		game.run();
		});

	menu->addOption("Play Random", [&]() {
		int result = dist(rng);
		if (result == 0) {
			Game game(window, Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::AlphaBeta, chessLevelIA);
			game.run();
		}
		else {
			Game game(window, Game::GameType::Chess, Player::PlayerType::AlphaBeta, Player::PlayerType::Human, chessLevelIA);
			game.run();
		}
		});

	menu->addOption("Main Menu", [&]() {
		setMainMenu();
		});
}

void MenuManager::setPower4Menu() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

	menu->addOption("Player vs Computer", [&]() {
		setPower4MenuVSComputer();
		});

	menu->addOption("Player vs Player", [&]() {
		Game game(window, Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::Human);
		game.run();
		});

	menu->addOption("Main Menu", [&]() {
		setMainMenu();
		});
}

void MenuManager::setPower4MenuVSComputer() {
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

	menu->addOption("Play first", [&]() {
		Game game(window, Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::AlphaBeta, power4LevelIA);
		game.run();
		});

	menu->addOption("Play second", [&]() {
		Game game(window, Game::GameType::Power4, Player::PlayerType::AlphaBeta, Player::PlayerType::Human, power4LevelIA);
		game.run();
		});

	menu->addOption("Play random", [&]() {
		int result = dist(rng);
		if (result == 0) {
			Game game(window, Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::AlphaBeta, power4LevelIA);
			game.run();
		}
		else {
			Game game(window, Game::GameType::Power4, Player::PlayerType::AlphaBeta, Player::PlayerType::Human, power4LevelIA);
			game.run();
		}
		});

	menu->addOption("Main Menu", [&]() {
		setMainMenu();
		});
}

void MenuManager::update(sf::Vector2i mousePos) {
	menu->update(mousePos);
}

void MenuManager::render(sf::RenderWindow* window)
{
	menu->render(window);
}

int MenuManager::handleEvent(sf::Event event)
{
	menu->handleEvent(event);
	return 0;
}
