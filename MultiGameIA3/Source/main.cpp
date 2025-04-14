#include "../Headers/MenuManager.h"
#include "../Headers/Game.h"

int main()
{
	sf::RenderWindow* window = new sf::RenderWindow();
	window->create(sf::VideoMode(700, 500), "Menu multigame", sf::Style::Titlebar | sf::Style::Close);
	window->setPosition(sf::Vector2i(0, 0));

	MenuManager* menuManager = new MenuManager(window);
	menuManager->run();
	delete menuManager;
	menuManager = nullptr;
	
	return EXIT_SUCCESS;
}