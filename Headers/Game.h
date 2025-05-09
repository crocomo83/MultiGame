#pragma once

#include "Player.h"
#include "ChessBoard.h"
#include "Graphics/IDrawable.h"
#include "Graphics/Menu.h"

#include <future>
#include <fstream>

enum class GameType {
	None,
	Chess,
	Power4
};

enum class GameState {
	Playing,
	Player1GiveUp,
	Player2GiveUp,
	Exit
};

class Game : public IDrawable {
	public :
							Game(GameType gameType, Player::PlayerType player1, Player::PlayerType player2, 
								const sf::Font* font, int level = -1);
							~Game();

		// IDrawable methods
		void				update(sf::Vector2i mousePosition) override;
		void				render(sf::RenderWindow& window) override;
		int					handleEvent(const std::optional<sf::Event> event) override;
		void				reset() override;

		GameState			getGameState() const;
		bool				isGameOver();

	private :
		static std::string  gameTypeToString(GameType gameType);
		void				writeLine(const std::string& texte);
		void				initSave();
		void				setMenu();
		void				swapPlayer();
		void				computePlay();
		void				checkGameOver();

		std::string			getMessageGameOver() const;

	private :
		GameType				gameType;
		Menu*					menu;
		std::string				msgGameOver;
		sf::Text*				gameOverText;
		const sf::Font*			font;
		GameState				gameState;
		bool					gameOver;

		std::string				fileName;
		std::ofstream			saveFile;
		sf::Vector2i			mousePos;
		int						currentPlayer;

		int						levelIA;
		BasicBoard*					board;
		Player::Data			players[2];
		Piece*					selectedPiece;

		int						currentMoveOnMouse;
		std::vector<int>		selectedMove;
		int						indexDecision;
};