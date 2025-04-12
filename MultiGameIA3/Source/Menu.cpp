#include "../Headers/Menu.h"
#include "../Headers/Game.h"
#include "../Headers/Player.h"

Menu::Menu(sf::RenderWindow* window_)
	: window(window_)
	, pos(0)
	, pressed(false)
	, theselect(false)
{
	font = new sf::Font();
	image = new sf::Texture();
	backgroundSprite = new sf::Sprite();

	pos_mouse = { 0,0 };
	mouse_coord = { 0, 0 };
}

Menu::~Menu() {
	delete font;
	delete image;
	delete backgroundSprite;
}

void Menu::addOption(const std::string& label, std::function<void()> action) {
	entries.emplace_back(label, action);
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
	float deltaPosY = 80;

	for (std::size_t i = 0; i < entries.size(); ++i) {
		sf::Text text;
		text.setFont(*font);
		text.setString(entries[i].first);
		text.setCharacterSize(24);
		text.setOutlineColor(sf::Color::Black);
		text.setPosition({ posX, initPosY + i * deltaPosY });
		texts.push_back(text);
		coords.push_back(text.getPosition());
	}

	for (auto& text : texts)
		text.setOutlineThickness(1);
}

void Menu::loop_events() {
	sf::Event event;
	while (window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			window->close();
		}

		pos_mouse = sf::Mouse::getPosition(*window);
		mouse_coord = window->mapPixelToCoords(pos_mouse);

		for (auto& text : texts)
			text.setOutlineThickness(1);

		pos = -1;
		for (int i = 0; i < texts.size(); ++i) {
			if (texts[i].getGlobalBounds().contains(mouse_coord)) {
				texts[i].setOutlineThickness(4);
				pos = i;
			}
		}

		if (event.type == sf::Event::MouseButtonReleased 
			&& event.mouseButton.button == sf::Mouse::Left 
			&& pos >= 0) 
		{
			entries[pos].second();
		}
	}
}


void Menu::draw_all() {
	window->clear();
	window->draw(*backgroundSprite);
	for (auto t : texts) {
		window->draw(t);
	}
	window->display();
}

void Menu::run_menu() {
	prepareMenu();
	while (window->isOpen()) {
		loop_events();
		draw_all();
	}
}