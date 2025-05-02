#include "../Headers/OptionSelector.h"

OptionSelector::OptionSelector(const std::vector<std::string>& labels,
    const sf::Font& font,
    sf::Vector2f startPos,
    float verticalSpacing,
    float radius)
    : selectedIndex(-1)
{
    for (size_t i = 0; i < labels.size(); ++i) {
        sf::CircleShape outerCircle(radius);
        outerCircle.setOutlineThickness(2.f);
        outerCircle.setOutlineColor(sf::Color::White);
        outerCircle.setFillColor(sf::Color::Transparent);
        outerCircle.setPosition(startPos.x, startPos.y + i * verticalSpacing);

        sf::CircleShape innerCircle(radius / 2.f);
        innerCircle.setFillColor(sf::Color::White);
        innerCircle.setOrigin(innerCircle.getRadius(), innerCircle.getRadius());
        innerCircle.setPosition(
            outerCircle.getPosition().x + radius,
            outerCircle.getPosition().y + radius
        );

        sf::Text text;
        text.setFont(font);
        text.setString(labels[i]);
        text.setCharacterSize(20);
        text.setPosition(outerCircle.getPosition().x + radius * 2 + 10, outerCircle.getPosition().y - 2);

        Option opt = { outerCircle, innerCircle, text };
        options.push_back(opt);
    }
}

int OptionSelector::handleEvent(const sf::Event& event) {
    if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
        for (size_t i = 0; i < options.size(); ++i) {
            if (options[i].outerCircle.getGlobalBounds().contains((sf::Vector2f)mousePos)) {
                selectedIndex = static_cast<int>(i);
            }
        }
    }
    return 0;
}

void OptionSelector::update(sf::Vector2i mousePosition)
{
    mousePos = mousePosition;
}

void OptionSelector::render(sf::RenderWindow& window) {
    for (size_t i = 0; i < options.size(); ++i) {
        window.draw(options[i].outerCircle);
        if (static_cast<int>(i) == selectedIndex)
            window.draw(options[i].innerCircle);
        window.draw(options[i].label);
    }
}

int OptionSelector::getSelectedIndex() const { 
    return selectedIndex; 
}