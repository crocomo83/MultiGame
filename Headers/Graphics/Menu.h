#pragma once

#include "IDrawable.h"

#include <SFML/Graphics.hpp>
#include <iostream>
#include <functional>

enum class Layout{
	Horizontal, 
	Vertical
};

class Menu : public IDrawable {
public:
	Menu(sf::Vector2f startPosition_, Layout layout, float spaceBetweenText_);
	~Menu();

	// IDrawable methods
	void update(sf::Vector2i mousePosition) override;
	void render(sf::RenderWindow& window) override;
	int	 handleEvent(const sf::Event& event) override;
	void reset() override;

	void addOption(const std::string& label, std::function<void()> action);
	void setSpaceBetweenText(float value);
	void clearOptions();
	void prepareMenu(const sf::Font& font);

private:
	sf::Vector2f startPosition;
	Layout layout;
	sf::Vector2f spaceBetweenText;
	int pos;

	std::vector<sf::Vector2f> coords;
	std::vector<sf::Text> texts;
	std::vector<std::pair<std::string, std::function<void()>>> entries;
};