#pragma once

#include <optional>

#include <SFML/Graphics/RenderWindow.hpp>

class IDrawable {
    public:
        virtual ~IDrawable() = default;

        virtual void update(sf::Vector2i mousePosition) = 0;
        virtual int  handleEvent(const std::optional<sf::Event> event) = 0;
        virtual void render(sf::RenderWindow& window) = 0;
        virtual void reset() = 0;
};