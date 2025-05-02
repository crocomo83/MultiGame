#pragma once

#include "IDrawable.h"

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

class OptionSelector : public IDrawable {
public:
    OptionSelector(const std::vector<std::string>& labels, const sf::Font& font, sf::Vector2f startPos, float verticalSpacing = 50.f, float radius = 15.f);

    // IDrawable methods
    void update(sf::Vector2i mousePosition) override;
    void render(sf::RenderWindow& window) override;
    int	 handleEvent(const sf::Event& event) override;

    int getSelectedIndex() const;

private:
    struct Option {
        sf::CircleShape outerCircle;
        sf::CircleShape innerCircle;
        sf::Text label;
    };

    std::vector<Option> options;
    int selectedIndex;
    sf::Vector2i mousePos;
};