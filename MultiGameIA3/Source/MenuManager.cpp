#include "../Headers/MenuManager.h"
#include "../Headers/Game.h"

const int chessLevelIA = 4;
const int power4LevelIA = 5;

MenuManager::MenuManager(sf::RenderWindow* window_)
	: window(window_)
	, rng(static_cast<unsigned int>(std::time(nullptr)))
	, dist(0, 1)
{
	menu = new Menu(window);
	setMainMenu();
}

MenuManager::~MenuManager() {
}

void MenuManager::setMainMenu() {
	menu->clearOptions();

	menu->addOption("Echecs", [&]() {
		setChessMenu();
		});

	menu->addOption("Puissance 4", [&]() {
		setPower4Menu();
		});

	menu->addOption("Quit", [&]() {
		window->close();
		});

	menu->prepareMenu();
}

void MenuManager::setChessMenu() {
	menu->clearOptions();

	menu->addOption("Player vs Computer", [&]() {
		setChessMenuVSComputer();
		});

	menu->addOption("Player vs Player", [&]() {
		Game game(Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::Human);
		game.run();
		});

	menu->addOption("Menu Principal", [&]() {
		setMainMenu();
		});

	menu->prepareMenu();
}

void MenuManager::setChessMenuVSComputer() {
	menu->clearOptions();

	menu->addOption("Player White", [&]() {
		Game game(Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::MinMax, chessLevelIA);
		game.run();
		});

	menu->addOption("Player Black", [&]() {
		Game game(Game::GameType::Chess, Player::PlayerType::MinMax, Player::PlayerType::Human, chessLevelIA);
		game.run();
		});

	menu->addOption("Player Random", [&]() {
		int result = dist(rng);
		if (result == 0) {
			Game game(Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::MinMax, chessLevelIA);
			game.run();
		}
		else {
			Game game(Game::GameType::Chess, Player::PlayerType::MinMax, Player::PlayerType::Human, chessLevelIA);
			game.run();
		}
		});

	menu->addOption("Menu Principal", [&]() {
		setMainMenu();
		});

	menu->prepareMenu();
}

void MenuManager::setPower4Menu() {
	menu->clearOptions();

	menu->addOption("Player vs Computer", [&]() {
		setPower4MenuVSComputer();
		});

	menu->addOption("Player vs Player", [&]() {
		Game game(Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::Human);
		game.run();
		});

	menu->addOption("Menu Principal", [&]() {
		setMainMenu();
		});

	menu->prepareMenu();
}

void MenuManager::setPower4MenuVSComputer() {
	menu->clearOptions();

	menu->addOption("Play first", [&]() {
		Game game(Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::MinMax, power4LevelIA);
		game.run();
		});

	menu->addOption("Play second", [&]() {
		Game game(Game::GameType::Power4, Player::PlayerType::MinMax, Player::PlayerType::Human, power4LevelIA);
		game.run();
		});

	menu->addOption("Play random", [&]() {
		int result = dist(rng);
		if (result == 0) {
			Game game(Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::MinMax, power4LevelIA);
			game.run();
		}
		else {
			Game game(Game::GameType::Power4, Player::PlayerType::MinMax, Player::PlayerType::Human, power4LevelIA);
			game.run();
		}
		});

	menu->addOption("Menu Principal", [&]() {
		setMainMenu();
		});

	menu->prepareMenu();
}

void MenuManager::run() {
	menu->run_menu();
}