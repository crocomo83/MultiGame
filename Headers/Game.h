#pragma once

#include "Player.h"
#include "ChessBoard.h"

#include <future>
#include <fstream>

class Game {
	public:
		enum GameType {
			Chess,
			Power4
		};

	public :
							Game(sf::RenderWindow* window_, GameType gameType, Player::PlayerType player1, Player::PlayerType player2, int level = -1);
							~Game();
		void				run();

	private :
		static std::string  gameTypeToString(GameType gameType);
		void				writeLine(const std::string& texte);
		void				initSave();
		void				swapPlayer();
		void				render();
		int					handleEvent();
		void				computePlay(int indexDecision);

	private :
		std::string				fileName;
		std::ofstream			saveFile;
		bool					gameOver;
		bool					hasSwap;
		sf::Vector2i			posMouse;
		int						currentPlayer;

		int						levelIA;
		sf::RenderWindow*		window;
		IBoard*					board;
		Player::Data			players[2];
		Piece*					selectedPiece;

		int									currentMoveOnMouse;
		std::vector<int>					selectedMove;
};