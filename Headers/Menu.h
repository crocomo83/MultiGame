#pragma once

#include "IDrawable.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

class Menu : public IDrawable {

public:
	Menu(float spaceBetweenText_);
	~Menu();

	// IDrawable methods
	void update(sf::Vector2i mousePosition) override;
	void render(sf::RenderWindow& window) override;
	int	 handleEvent(const sf::Event& event) override;

	void addOption(const std::string& label, std::function<void()> action);
	void setSpaceBetweenText(float value);
	void clearOptions();
	void prepareMenu();

private:
	float spaceBetweenText;
	int pos;
	bool pressed, theselect;

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