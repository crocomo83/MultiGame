#pragma once

#include "IDrawable.h"
#include "Menu.h"

#include <SFML/Graphics.hpp>

enum class AppState {
    MainMenu,
    ChessMenu,
    Power4Menu,
    ReplayMenu,
    Game,
    Replay,
    Exit
};

class MainLoop {
    public:
             MainLoop(sf::RenderWindow* window_);
             ~MainLoop();

        void run();
        void render();
        void handleEvent();

    private:
        Menu* createMainMenu();

    private:
        AppState appState;
        sf::RenderWindow* window;
        std::map<AppState, IDrawable*> states;
        sf::Vector2i mousePos;
};