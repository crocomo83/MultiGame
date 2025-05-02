#include "../Headers/MainLoop.h"
#include "../Headers/Utility.h"

MainLoop::MainLoop(sf::RenderWindow* window_)
: appState(AppState::MainMenu)
, window(window_)
{
	states.emplace(AppState::MainMenu, createMainMenu());
	states.emplace(AppState::ChessMenu, createMainMenu());
	states.emplace(AppState::Power4Menu, createMainMenu());
	states.emplace(AppState::ReplayMenu, createMainMenu());
}

MainLoop::~MainLoop()
{
}

void MainLoop::run()
{
	while (window->isOpen())
	{
		handleEvent();
		if (appState == AppState::Exit) {
			return;
		}
		states[appState]->update(mousePos);
		render();
		
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
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window->close();
		}
		else if(event.type == sf::Event::MouseMoved) {
			mousePos.x = event.mouseMove.x;
			mousePos.y = event.mouseMove.y;
		}
		else {
			states[appState]->handleEvent(event);
		}
	}
}

Menu* MainLoop::createMainMenu() {
	Menu* menu = new Menu(spaceBetweenTextMenu);
	menu->clearOptions();
	menu->setSpaceBetweenText(spaceBetweenTextMenu);

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
	menu->prepareMenu();
	return menu;
}
