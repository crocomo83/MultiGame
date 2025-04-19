#pragma once

#include "Player.h"
#include "ChessBoard.h"

class Game {
	public:
		enum GameType {
			Chess,
			Power4
		};

	public :
							Game(GameType gameType, Player::PlayerType player1, Player::PlayerType player2, int level = -1);
		void				run();

	private :
		void				swapPlayer();
		void				render();
		void				handleEvent();
		void				handleActionEvent(sf::Event &event);
		void				computePlay();

	private :
		bool					gameOver;
		bool					hasSwap;
		int						xMouse;
		int						yMouse;
		int						currentPlayer;

		int						levelIA;
		sf::RenderWindow*		window;
		IBoard*					board;
		Player::Data			players[2];
		Piece*					selectedPiece;

		int									currentMoveOnMouse;
		std::vector<int>					selectedMove;
};