#include "../../Headers/Graphics/Menu.h"

Menu::Menu(sf::Vector2f startPosition_, Layout layout_, float spaceBetweenText_)
: startPosition(startPosition_)
, layout(layout_)
, pos(0)
, spaceBetweenText({0, 0})
{
	setSpaceBetweenText(spaceBetweenText_);
}

Menu::~Menu() {
}

void Menu::addOption(const std::string& label, std::function<void()> action) {
	entries.emplace_back(label, action);
}

void Menu::setSpaceBetweenText(float value) {
	if (layout == Layout::Horizontal) {
		spaceBetweenText.x = value;
	}
	else {
		spaceBetweenText.y = value;
	}
}

void Menu::clearOptions() {
	entries.clear();
}

void Menu::prepareMenu(const sf::Font& font) {
	texts.clear();
	coords.clear();

	for (std::size_t i = 0; i < entries.size(); ++i) {
		sf::Text text;
		text.setFont(font);
		text.setString(entries[i].first);
		text.setCharacterSize(24);
		text.setOutlineColor(sf::Color::Black);
		text.setPosition({ startPosition.x + i * spaceBetweenText.x, startPosition.y + i * spaceBetweenText.y });
		texts.push_back(text);
		coords.push_back(text.getPosition());
	}

	for (auto& text : texts) {
		text.setOutlineThickness(1);
	}
}

int Menu::handleEvent(const sf::Event& event) {
	if (event.type == sf::Event::MouseButtonReleased
		&& event.mouseButton.button == sf::Mouse::Left
		&& pos >= 0)
	{
		entries[pos].second();
	}
	return 0;
}

void Menu::reset()
{
}


void Menu::render(sf::RenderWindow& window) {
	for (auto &t : texts) {
		window.draw(t);
	}
}

void Menu::update(sf::Vector2i mousePosition) {
	for (auto& text : texts) {
		text.setOutlineThickness(1);
	}

	pos = -1;
	for (int i = 0; i < texts.size(); ++i) {
		if (texts[i].getGlobalBounds().contains((sf::Vector2f)mousePosition)) {
			texts[i].setOutlineThickness(4);
			pos = i;
		}
	}
}