#include "../Headers/MainLoop.h"
#include "../Headers/Utility.h"
#include "../Headers/Player.h"

#include "../Headers/Graphics/Image.h"
#include "../Headers/Graphics/OptionSelector.h"

#include <filesystem>

MainLoop::MainLoop(sf::RenderWindow* window_)
: appState(AppState::MainMenu)
, window(window_)
{
	initFont();
	initBackgroundImage();

	states.emplace(AppState::MainMenu, createMainMenu());
	states.emplace(AppState::ChessMenu, createGameMenu(AppState::ChessMenu));
	states.emplace(AppState::Power4Menu, createGameMenu(AppState::Power4Menu));
	states.emplace(AppState::ReplayMenu, createReplayMenu());
	states.emplace(AppState::Game, nullptr);
	states.emplace(AppState::Replay, nullptr);
}

MainLoop::~MainLoop()
{
}

void MainLoop::initFont() {
	font = new sf::Font();
	if (!font->openFromFile("Media/arial.ttf"))
	{
		std::cout << "error while loading font" << std::endl;
	}
}

void MainLoop::initBackgroundImage() {
	image = new sf::Texture();
	if (!image->loadFromFile("Media/Main_menu.png")) {
		std::cout << "error while loading background image" << std::endl;
	}
}

void MainLoop::run()
{
	while (window->isOpen())
	{
		handleEvent();
		if (appState == AppState::Exit) {
			return;
		}
		else {
			updateAppState();
		}
		states[appState]->update(mousePos);
		render();
		
	}
}

void MainLoop::updateAppState() {
	if (appState == AppState::Game) {
		Game* game = dynamic_cast<Game*>(states[appState]);
		GameState state = game->getGameState();
		if (state == GameState::Exit) {
			appState = AppState::MainMenu;
			game->reset();
		}
	}
}

void MainLoop::render()
{
	window->clear(sf::Color::White);
	states[appState]->render(*window);
	window->display();
}

void MainLoop::handleEvent()
{
	while (const std::optional event = window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
		{
			appState = AppState::Exit;
		}
		else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>())
		{
			mousePos = mouseMoved->position;
		}
		else {
			states[appState]->handleEvent(event);
		}
	}
}

Container* MainLoop::createMainMenu() {
	Container* container = new Container();

	Image* backgroundImage = new Image(*image);
	container->addWidget(backgroundImage);

	Menu* menu = new Menu({ 250, 150 }, Layout::Vertical, spaceBetweenTextMenu);

	menu->addOption("Chess", [&]() {
		appState = AppState::ChessMenu;
		});

	menu->addOption("Power 4", [&]() {
		appState = AppState::Power4Menu;
		});

	menu->addOption("Replay", [&]() {
		appState = AppState::ReplayMenu;
		});

	menu->addOption("Quit", [&]() {
		appState = AppState::Exit;
		});
	menu->prepareMenu(*font);

	container->addWidget(menu);

	return container;
}

Container* MainLoop::createGameMenu(AppState appStateMenu) {
	Container* container = new Container();

	Image* backgroundImage = new Image(*image);
	container->addWidget(backgroundImage);

	std::vector<std::string> labels;
	labels.push_back("Player vs IA");
	labels.push_back("Player vs Player");
	OptionSelector* opponentSelector = new OptionSelector(labels, *font, { 250, 150 });
	container->addWidget(opponentSelector);

	std::vector<std::string> labels2;
	labels2.push_back("Play first");
	labels2.push_back("Play second");
	labels2.push_back("Play random");
	OptionSelector* orderSelector = new OptionSelector(labels2, *font, { 250, 300 });
	container->addWidget(orderSelector);

	opponentSelector->setOnItemChanged([orderSelector](int index) {
		orderSelector->setEnabled(index == 0);
	});

	Menu* menu = new Menu({ 150, 500 }, Layout::Horizontal, 300);

	menu->addOption("Play", [=]() {
		GameType associatedGameType = menuToGameType(appState);
		
		Player::PlayerType player1 = Player::PlayerType::Human;
		Player::PlayerType player2 = Player::PlayerType::Human;
		if (opponentSelector->getSelectedIndex() == 0) {
			player2 = Player::PlayerType::AlphaBeta;
			if (orderSelector->getSelectedIndex() == 0) {
				states[AppState::Game] = new Game(associatedGameType, player1, player2, font, getLevelIA(associatedGameType));
			}
			else {
				states[AppState::Game] = new Game(associatedGameType, player2, player1, font, getLevelIA(associatedGameType));
			}
		}
		else {
			states[AppState::Game] = new Game(associatedGameType, player1, player2, font, getLevelIA(associatedGameType));
		}
		appState = AppState::Game;
		container->reset();
		});

	menu->addOption("Main menu", [&]() {
		appState = AppState::MainMenu;
		});
	menu->prepareMenu(*font);
	container->addWidget(menu);

	return container;
}

Container* MainLoop::createReplayMenu() {
	Container* container = new Container();

	Image* backgroundImage = new Image(*image);
	container->addWidget(backgroundImage);

	std::vector<std::string> labels = listFilesInDirectory(std::string(SAVES_STR));
	OptionSelector* listFileSelector = new OptionSelector(labels, *font, { 150, 140 }, 9, 40.f, 15.f);
	container->addWidget(listFileSelector);

	Menu* menu = new Menu({ 100, 500 }, Layout::Horizontal, 400);

	menu->addOption("Replay", [=]() {
		Container * replayContainer = new Container();

		Replayer* replayer = new Replayer(listFileSelector->getText(listFileSelector->getSelectedIndex()));
		replayContainer->addWidget(replayer);

		Menu * replayMenu = new Menu({ 530, 80 }, Layout::Vertical, 60);

		replayMenu->clearOptions();

		replayMenu->addOption("Replay menu", [&]() {
			appState = AppState::ReplayMenu;
			});

		replayMenu->addOption("Main menu", [&]() {
			appState = AppState::MainMenu;
			});
		replayMenu->prepareMenu(*font);
		replayContainer->addWidget(replayMenu);
		
		states[AppState::Replay] = replayContainer;

		appState = AppState::Replay;
		container->reset();
		});

	menu->addOption("Main menu", [&]() {
		appState = AppState::MainMenu;
		});
	menu->prepareMenu(*font);
	container->addWidget(menu);

	return container;
}

std::vector<std::string> MainLoop::listFilesInDirectory(const std::string& folderPath) {
	std::vector<std::string> fileNames;

	try {
		for (const auto& entry : std::filesystem::directory_iterator(folderPath)) {
			if (entry.is_regular_file()) {
				fileNames.push_back(entry.path().filename().string());
			}
		}
	}
	catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Erreur lors de l'acc s au dossier : " << e.what() << std::endl;
	}

	return fileNames;
}

GameType MainLoop::menuToGameType(AppState state) const{
	switch (state) {
		case AppState::ChessMenu:
			return GameType::Chess;
		case AppState::Power4Menu:
			return GameType::Power4;
		default:
			return GameType::None;
	}
}

int MainLoop::getLevelIA(GameType gameType) const {
	switch (gameType) {
		case GameType::Chess:
			return 4;
		case GameType::Power4:
			return 6;
		default:
			return -1;
	}
}