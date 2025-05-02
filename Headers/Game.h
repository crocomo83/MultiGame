#pragma once

#include "Player.h"
#include "ChessBoard.h"
#include "IDrawable.h"

#include <future>
#include <fstream>

class Game : public IDrawable {
	public:
		enum GameType {
			Chess,
			Power4
		};

	public :
							Game(GameType gameType, Player::PlayerType player1, Player::PlayerType player2, int level = -1);
							~Game();

		// IDrawable methods
		void				update(sf::Vector2i mousePosition) override;
		void				render(sf::RenderWindow& window) override;
		int					handleEvent(const sf::Event& event) override;

	private :
		static std::string  gameTypeToString(GameType gameType);
		void				writeLine(const std::string& texte);
		void				initSave();
		void				swapPlayer();
		void				computePlay(int indexDecision);

	private :
		std::string				fileName;
		std::ofstream			saveFile;
		bool					gameOver;
		bool					hasSwap;
		sf::Vector2i			mousePos;
		int						currentPlayer;

		int						levelIA;
		IBoard*					board;
		Player::Data			players[2];
		Piece*					selectedPiece;

		int									currentMoveOnMouse;
		std::vector<int>					selectedMove;
};