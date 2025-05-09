#include "../../Headers/Graphics/OptionSelector.h"

#include <iostream>
#include <algorithm>

OptionSelector::OptionSelector(const std::vector<std::string>& labels, const sf::Font& font, sf::Vector2f startPos_, 
    int nbItemRenderedMax_, float verticalSpacing_, float radius_)
: selectedIndex(0)
, enabled(true)
, startPos(startPos_)
, firstItemDisplayed(0)
, verticalSpacing(verticalSpacing_)
, radius(radius_)
, nbItemRenderedMax(nbItemRenderedMax_)
, scrollBar(startPos_ - sf::Vector2f(radius_, 2), nbItemRenderedMax_ * verticalSpacing_)
{
    for (size_t i = 0; i < labels.size(); ++i) {
        sf::CircleShape outerCircle(radius);
        outerCircle.setOutlineThickness(2.f);
        outerCircle.setOutlineColor(sf::Color::Black);
        outerCircle.setFillColor(sf::Color::Transparent);
        outerCircle.setPosition({ startPos.x, startPos.y + i * verticalSpacing });

        sf::CircleShape innerCircle(radius / 2.f);
        innerCircle.setFillColor(sf::Color::Black);
        innerCircle.setOrigin({ innerCircle.getRadius(), innerCircle.getRadius() });
        innerCircle.setPosition({ outerCircle.getPosition().x + radius, outerCircle.getPosition().y + radius });

        sf::Text text(font, labels[i], 20);
        text.setFillColor(sf::Color::Black);
        text.setPosition({ outerCircle.getPosition().x + radius * 2 + 10, outerCircle.getPosition().y - 2 });

        Option opt = { outerCircle, innerCircle, text };
        options.push_back(opt);
    }
    scrollBar.setHeightInnerEllipsePercent((float)nbItemRenderedMax / (float)labels.size() * 100);
}

int OptionSelector::handleEvent(const std::optional<sf::Event> event) {
    if (!enabled) {
        return 0;
    }
    if (const auto* mouseReleased = event->getIf<sf::Event::MouseButtonPressed>())
    {
        if (mouseReleased->button == sf::Mouse::Button::Left) {
            for (int i = 0; i < options.size(); i++) {
                if (options[i].outerCircle.getGlobalBounds().contains((sf::Vector2f)mousePos)) {
                    if (selectedIndex != i) {
                        selectedIndex = i;
                        if (onItemChanged) {
                            onItemChanged(i);
                        }
                    }
                }
            }
        }
    }
    else if (const auto* mouseScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
        float delta = mouseScrolled->delta;
        if (delta < 0) {
            if (firstItemDisplayed + nbItemRenderedMax < options.size()) {
                firstItemDisplayed++;
            }
        }
        else {
            if (firstItemDisplayed > 0) {
                firstItemDisplayed--;
            }
        }
        updateOptionsPosition();
        scrollBar.setPositionInnerEllipsePercent((float)firstItemDisplayed / (float)options.size());
    }
    return 0;
}

void OptionSelector::update(sf::Vector2i mousePosition)
{
    mousePos = mousePosition;
}

void OptionSelector::render(sf::RenderWindow& window) {
    for (int i = firstItemDisplayed; i < firstItemDisplayed + std::min(nbItemRenderedMax, static_cast<int>(options.size())); ++i) {
        window.draw(options[i].outerCircle);
        if (static_cast<int>(i) == selectedIndex && enabled) {
            window.draw(options[i].innerCircle);
        }
        window.draw(options[i].label);
    }
    if (static_cast<int>(options.size()) > nbItemRenderedMax) {
        scrollBar.render(window);
    }
}

void OptionSelector::reset()
{
    selectedIndex = 0;
    enabled = true;
}

void OptionSelector::updateOptionsPosition() {
    for (size_t i = 0; i < options.size(); ++i) {
        float posX = startPos.x;
        float posY = startPos.y + (static_cast<int>(i) - firstItemDisplayed) * verticalSpacing;
        options[i].outerCircle.setPosition({ posX, posY });
        options[i].innerCircle.setPosition({ posX + radius, posY + radius });
        options[i].label.setPosition({ posX + radius * 2 + 10, posY - 2 });
    }
}

int OptionSelector::getSelectedIndex() const { 
    return selectedIndex; 
}

std::string OptionSelector::getText(int index) const
{
    return options[index].label.getString();
}

void OptionSelector::setSelectedIndex(int index) {
    selectedIndex = index;
}

bool OptionSelector::isEnabled() const
{
    return enabled;
}

void OptionSelector::setEnabled(bool enabled_) {
    enabled = enabled_;
    sf::Color fillColor = (enabled ? sf::Color::Black : sf::Color(150, 150, 150));
    for (size_t i = 0; i < options.size(); ++i) {
        options[i].outerCircle.setOutlineColor(fillColor);;
        options[i].innerCircle.setFillColor(fillColor);
        options[i].label.setFillColor(fillColor);
    }
}

void OptionSelector::setOnItemChanged(std::function<void(int)> callback)
{
    onItemChanged = std::move(callback);
}