#include "../Headers/MainLoop.h"
#include "../Headers/Game.h"

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow();
	window->create(sf::VideoMode(700, 600), "Multigame", sf::Style::Titlebar | sf::Style::Close);
	window->setPosition(sf::Vector2i(0, 0));

	MainLoop* mainLoop = new MainLoop(window);
	mainLoop->run();

	delete mainLoop;
	mainLoop = nullptr;
	
	return EXIT_SUCCESS;
}