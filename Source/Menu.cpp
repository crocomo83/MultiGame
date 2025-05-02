#include "../Headers/Menu.h"
#include "../Headers/Game.h"
#include "../Headers/Player.h"

Menu::Menu(float spaceBetweenText_)
: pos(0)
, pressed(false)
, theselect(false)
, spaceBetweenText(spaceBetweenText_)
{
	font = new sf::Font();
	image = new sf::Texture();
	backgroundSprite = new sf::Sprite();

	pos_mouse = {0, 0};
	mouse_coord = {0, 0};
}

Menu::~Menu() {
	delete font;
	delete image;
	delete backgroundSprite;
}

void Menu::addOption(const std::string& label, std::function<void()> action) {
	entries.emplace_back(label, action);
}

void Menu::setSpaceBetweenText(float value) {
	spaceBetweenText = value;
}

void Menu::clearOptions() {
	entries.clear();
}

void Menu::prepareMenu() {
	font->loadFromFile("Media/arial.ttf");
	image->loadFromFile("Media/Main_menu.png");
	backgroundSprite->setTexture(*image);

	texts.clear();
	coords.clear();

	float posX = 250;
	float initPosY = 150;

	for (std::size_t i = 0; i < entries.size(); ++i) {
		sf::Text text;
		text.setFont(*font);
		text.setString(entries[i].first);
		text.setCharacterSize(24);
		text.setOutlineColor(sf::Color::Black);
		text.setPosition({ posX, initPosY + i * spaceBetweenText });
		texts.push_back(text);
		coords.push_back(text.getPosition());
	}

	for (auto& text : texts) {
		text.setOutlineThickness(1);
	}
}

int Menu::handleEvent(const sf::Event& event) {
	for (auto& text : texts) {
		text.setOutlineThickness(1);
	}
		
	pos = -1;
	for (int i = 0; i < texts.size(); ++i) {
		if (texts[i].getGlobalBounds().contains((sf::Vector2f)pos_mouse)) {
			texts[i].setOutlineThickness(4);
			pos = i;
			std::cout << "set pos : " << i << std::endl;
		}
	}

	if (event.type == sf::Event::MouseButtonReleased
		&& event.mouseButton.button == sf::Mouse::Left
		&& pos >= 0)
	{
		entries[pos].second();
	}
	return 0;
}


void Menu::render(sf::RenderWindow& window) {
	window.draw(*backgroundSprite);
	for (auto t : texts) {
		window.draw(t);
	}
}

void Menu::update(sf::Vector2i mousePosition) {
	pos_mouse = mousePosition;
}