#pragma once

#include "ScrollBar.h"
#include "IDrawable.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Window/Event.hpp>

#include <vector>
#include <string>
#include <functional>

class OptionSelector : public IDrawable {
public:
    OptionSelector(const std::vector<std::string>& labels, const sf::Font& font, sf::Vector2f startPos, 
        int endPosY_ = 1000000, float verticalSpacing = 50.f, float radius = 15.f);

    // IDrawable methods
    void update(sf::Vector2i mousePosition) override;
    void render(sf::RenderWindow& window) override;
    int	 handleEvent(const sf::Event& event) override;
    void reset() override;

    void updateOptionsPosition();

    int getSelectedIndex() const;
    std::string getText(int index) const;
    void setSelectedIndex(int index);
    bool isEnabled() const;
    void setEnabled(bool enabled_);

    void setOnItemChanged(std::function<void(int)> callback);

private:
    struct Option {
        sf::CircleShape outerCircle;
        sf::CircleShape innerCircle;
        sf::Text label;
    };

    ScrollBar scrollBar;

    bool enabled;
    std::vector<Option> options;
    int selectedIndex;
    sf::Vector2i mousePos;

    sf::Vector2f startPos;
    int firstItemDisplayed;
    int nbItemRenderedMax;
    float verticalSpacing;
    float radius;

    std::function<void(int)> onItemChanged;
};