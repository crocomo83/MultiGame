#pragma once

#include "Player.h"
#include "Board.h"

class Game {

	public :
							Game();
							Game(Player::PlayerType whitePlayer, Player::PlayerType blackPlayer);
		void				run();

	private :
		void				swapPlayer();
		void				render();
		void				handleEvent();
		void				handleActionEvent(sf::Event &event);
		void				computePlay();
		void				checkGameOver();

	private :
		bool				debug;
		bool				hasSwap;
		int					xMouse;
		int					yMouse;
		int					currentPlayer;

		sf::RenderWindow	window;
		Board*				board;
		Player*				players[2];
		Piece*				selectedPiece;
};