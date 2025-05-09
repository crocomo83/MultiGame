#pragma once

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "IDrawable.h"

class Image : public IDrawable {

public:
	Image(const sf::Texture& texture);
	~Image();

	// IDrawable methods
	void	update(sf::Vector2i mousePosition) override;
	void	render(sf::RenderWindow& window) override;
	int		handleEvent(const std::optional<sf::Event> event) override;
	void	reset() override;

private:
	sf::Sprite* image;
};