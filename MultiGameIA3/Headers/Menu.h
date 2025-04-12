#pragma once

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

class Menu {

public:
	Menu(sf::RenderWindow* window);
	~Menu();
	void addOption(const std::string& label, std::function<void()> action);
	void clearOptions();
	void run_menu();
	void prepareMenu();

protected:
	void loop_events();
	void draw_all();

private:
	int pos;
	bool pressed, theselect;

	sf::RenderWindow* window;
	sf::Font* font;
	sf::Texture* image;
	sf::Sprite* backgroundSprite;

	sf::Vector2i pos_mouse;
	sf::Vector2f mouse_coord;

	std::vector<const char*> options;
	std::vector<sf::Vector2f> coords;
	std::vector<sf::Text> texts;
	std::vector<sf::RectangleShape> rectangles;
	std::vector<std::size_t> sizes;
	std::vector<std::pair<std::string, std::function<void()>>> entries;
};