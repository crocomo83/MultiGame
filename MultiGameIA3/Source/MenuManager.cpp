#include "../Headers/MenuManager.h"
#include "../Headers/Game.h"

MenuManager::MenuManager(sf::RenderWindow* window_)
	: window(window_)
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
		Game game(Game::GameType::Chess, Player::PlayerType::Human, Player::PlayerType::MinMax);
		game.run();
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

void MenuManager::setPower4Menu() {
	menu->clearOptions();

	menu->addOption("Player vs Computer", [&]() {
		Game game(Game::GameType::Power4, Player::PlayerType::Human, Player::PlayerType::MinMax);
		game.run();
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

void MenuManager::run() {
	menu->run_menu();
}