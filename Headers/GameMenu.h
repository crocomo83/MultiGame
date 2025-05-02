#pragma once

#include "IDrawable.h"

#include <SFML/Graphics.hpp>


class GameMenu : public IDrawable {

public:
	GameMenu();
	~GameMenu();

	// IDrawable methods
	void				update(sf::Vector2i mousePosition) override;
	void				render(sf::RenderWindow& window) override;
	int					handleEvent(const sf::Event& event) override;
};