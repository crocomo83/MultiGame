#pragma once

#include "Graphics/IDrawable.h"
#include "Graphics/Menu.h"
#include "Graphics/Container.h"
#include "Game.h"
#include "Replayer.h"

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

        void initFont();
        void initBackgroundImage();

        void run();
        void render();
        void handleEvent();

        void updateAppState();
        GameType menuToGameType(AppState state) const;
        int getLevelIA(GameType gameType) const;

    private:
        Container* createMainMenu();
        Container* createGameMenu(AppState appStateMenu);
        Container* createReplayMenu();

        std::vector<std::string> listFilesInDirectory(const std::string& folderPath);

    private:
        sf::Font* font;
        sf::Texture* image;
        AppState appState;
        sf::RenderWindow* window;
        std::map<AppState, IDrawable*> states;
        sf::Vector2i mousePos;
};