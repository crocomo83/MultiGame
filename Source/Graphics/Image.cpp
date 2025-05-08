#include "../../Headers/Graphics/Image.h"

Image::Image(const sf::Texture& texture)
: image(new sf::Sprite(texture))
{
}

Image::~Image()
{
	delete image;
}

void Image::update(sf::Vector2i mousePosition)
{
}

void Image::render(sf::RenderWindow& window)
{
	window.draw(*image);
}

int Image::handleEvent(const sf::Event& event)
{
	return 0;
}

void Image::reset()
{
}
