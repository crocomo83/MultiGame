#include "../Headers/Menu.h"
#include "../Headers/Game.h"
#include "../Headers/Player.h"

Menu::Menu() {
    window = new sf::RenderWindow();
    winclose = new sf::RectangleShape();
    font = new sf::Font();
    image = new sf::Texture();
    bg = new sf::Sprite();

    set_values();
}

Menu::~Menu() {
    delete window;
    delete winclose;
    delete font;
    delete image;
    delete bg;
}

void Menu::set_values() {
    window->create(sf::VideoMode(700, 500), "Chess master", sf::Style::Titlebar | sf::Style::Close);
    window->setPosition(sf::Vector2i(0, 0));

    pos = 0;
    pressed = theselect = false;
    font->loadFromFile("Media/arial.ttf");
    image->loadFromFile("Media/Main_menu.png");

    bg->setTexture(*image);

    pos_mouse = { 0,0 };
    mouse_coord = { 0, 0 };

    options = { "Player vs Computer", "Player vs Player", "Debug", "Quit" };
    texts.resize(4);
    float posX = 250;
    float initPosY = 150;
    float deltaPosY = 80;
    coords = { 
        {posX,initPosY},
        {posX,initPosY + deltaPosY},
        {posX,initPosY + 2 * deltaPosY},
        {posX,initPosY + 3 * deltaPosY}
    };

    sf::RectangleShape rectangle(sf::Vector2f(posX, initPosY));
    rectangle.setSize(sf::Vector2f(200, deltaPosY));
    rectangle.setFillColor(sf::Color(100, 250, 50));

    for (std::size_t i{}; i < texts.size(); ++i) {
        texts[i].setFont(*font);
        texts[i].setString(options[i]);
        texts[i].setCharacterSize(24);
        texts[i].setOutlineColor(sf::Color::Black);
        texts[i].setPosition(coords[i]);
    }
    pos = 0;

    winclose->setSize(sf::Vector2f(23, 26));
    winclose->setPosition(200, 200);
    winclose->setFillColor(sf::Color::Transparent);

}

void Menu::loop_events() {
    sf::Event event;
    while (window->pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window->close();
        }

        pos_mouse = sf::Mouse::getPosition(*window);
        mouse_coord = window->mapPixelToCoords(pos_mouse);

        texts[0].setOutlineThickness(1);
        texts[1].setOutlineThickness(1);
        texts[2].setOutlineThickness(1);
        texts[3].setOutlineThickness(1);

        for (int i = 0; i < texts.size(); i++) {
            if (texts[i].getGlobalBounds().contains(mouse_coord)) {
                texts[i].setOutlineThickness(4);
                pos = i + 1;
            }
        }

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            if (pos == 1) {
                Game game(Player::PlayerType::Human, Player::PlayerType::MinMax);
                game.run();
            }
            else if (pos == 2) {
                Game game(Player::PlayerType::Human, Player::PlayerType::Human);
                game.run();
            }
            else if (pos == 3) {
                Game game;
                game.run();
            }
            
            else if (pos == 4 || winclose->getGlobalBounds().contains(mouse_coord)) {
                std::cout << "Close the window!" << '\n';
                window->close();
            }
        }
    }
}

void Menu::draw_all() {
    window->clear();
    window->draw(*bg);
    for (auto t : texts) {
        window->draw(t);
    }
    window->display();
}

void Menu::run_menu() {
    while (window->isOpen()) {
        loop_events();
        draw_all();
    }
}