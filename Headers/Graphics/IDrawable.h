#pragma once

#include <SFML/Graphics.hpp>

class IDrawable {
    public:
        virtual ~IDrawable() = default;

        virtual void update(sf::Vector2i mousePosition) = 0;
        virtual int  handleEvent(const sf::Event& event) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void reset() = 0;
};