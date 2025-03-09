#pragma once

#include "Player.h"
#include "Board.h"
#include "Node.h"

class Game {

	public :
							Game();
							Game(Player::PlayerType whitePlayer, Player::PlayerType blackPlayer);
		void				run();

	private :
		void				initDebug();
		void				initFont();

		void				swapPlayer();
		void				render();
		void				handleEvent();
		void				handleActionEvent(sf::Event &event);
		void				computePlay();
		void				checkGameOver();
		void				computeDebug();
		void				updateDebug();

	private :
		bool					debug;
		bool					hasSwap;
		int						xMouse;
		int						yMouse;
		int						currentPlayer;

		sf::RenderWindow*		window;
		Board*					board;
		Player::Data			players[2];
		Piece*					selectedPiece;

		Node*					lastIAPlay;

		sf::Font*							font;
		std::vector<std::vector<sf::Text>>	moveTexts;
		int									currentMoveOnMouse;
		std::vector<int>					selectedMove;
};