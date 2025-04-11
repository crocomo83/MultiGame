#pragma once

#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <stack>

#include "Piece.h"
#include "Utility.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>

class Board {

	public : 
		enum State {
			Normal,
			CheckMateWhite, 
			CheckMateBlack, 
			Equality
		};

	public :
		struct Highlight {
			bool activated;
			sf::Sprite sprite;
		};

	public : 
								Board();

		Piece*					select(int x, int y, int player);
		bool					unselect(int x, int y, Piece* piece);

		// Interface
		void					draw(sf::RenderWindow& target);
		void					update(int x, int y, int idPlayer);
		void					updateDebug();

		// Test de situations
		bool					isCheck(int idPlayer);
		bool					isCheckMate();
		bool					isEquality();
		State					getGameState();
		
		void					movePiece(Move &move);
		void					computeValidMoves(int idPlayer);
		void					printValidMoves();
		bool					play(Move &move, bool checkValidity);
		bool					undo();
		void					unMovePiece(Move &move);

		Piece*					getPiece(int x, int y) const;
		Piece*					getPiece(sf::Vector2i pos) const;

		void					addMove(int startX, int startY, int endX, int endY, int player, std::vector<Move> &moves, bool checkConsidered);
		bool					isValidMove(Move move);
		bool					isCorrectMove(Move move) const;
		void					getMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		std::vector<Move>		getMoves(int x, int y, bool checkConsidered);
		std::vector<Move>		getAllMoves(int idPlayer, bool checkConsidered);
		bool					isAnyMovePossible(int idPlayer);

		float					getEvaluation();

		std::string				getMoveSymbol(Move move);
		void					printMove(Move move);

		uint64_t				hashBoard() const;

	private :
		void					init();
		void					initSprites();
		void					initFont();
		void					initHighlights();
		void					initializeZobristTable();
		void					initDebug();

		void					setSpritePosition(sf::Sprite& sprite, sf::Vector2i pos);
		void					resetHighlights() const;

		bool					testMove(Move &move);

		float					eval();
		float					evalPieces() const;
		float					evalMoves();

		void					undo(Move &move);
		void					undo(Move &move, Move &previousMove);

		bool					isThreatenedBy(sf::Vector2i pos, int idPlayer) const;

		void					resetEnPassant(int idPlayer) const;
		void					getPawnMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		void					getKnightMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		void					getBishopMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		void					getTowerMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		void					getKingMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered);
		void					getDirectionMove(Piece* movingPiece, sf::Vector2i dir, std::vector<Move>& moves, bool checkConsidered);

		// Lecture ecriture
		std::string				generateBoardId() const;
		
	private :
		sf::Vector2i					selectedPawn;
		sf::Vector2f					mousePos;

		std::stack<short>				repetitiveMoves;

		Highlight*						highlights[8][8];
		Piece*							pieces[2][16];
		Piece*							piecesOnBoard[8][8];
		std::vector<Move>				history;
		std::vector<std::string>		historyBoard;
		std::vector<Move>				validMoves;

		sf::Texture						boardTexture;
		sf::Texture						piecesTexture;
		sf::Texture						highlightBlue;
		sf::Texture						highlightGreen;
		sf::Texture						highlightRed;

		sf::Font*						font;

		sf::Sprite						boardSprite;
		sf::Sprite						pieceSprites[2][6];
		sf::Sprite						blueSprite;
		sf::Sprite						greenSprite;
		sf::Sprite						redSprite;

		std::map<std::string, sf::Text> debugTexts;

		std::unordered_map<uint64_t, float> zobristCache;
		uint64_t zobristTable[64][12];
};
