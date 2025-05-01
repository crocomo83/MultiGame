#pragma once

#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <stack>

#include "Piece.h"
#include "Utility.h"
#include "IBoard.h"

#include <SFML\Graphics\Sprite.hpp>
#include <SFML\Graphics\Texture.hpp>
#include <SFML\Graphics\RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class ChessBoard : public IBoard {

	public : 
		enum State {
			Normal,
			CheckMateWhite, 
			CheckMateBlack, 
			Equality
		};

		enum Color {
			Blue,
			Green,
			Red,
			White,
			Black
		};

	public : 
								ChessBoard(bool reverseBoard_);

		void					select(sf::Vector2i mousePos);
		int						unselect(sf::Vector2i mousePos);

		bool					isGameOver(std::string& messageGameOver);
		int						handleEvent(sf::Vector2i mousePos, sf::Event& event);

		// Interface
		void					draw(sf::RenderWindow& target);
		void					update(sf::Vector2i pos, int idPlayer);

		// Test de situations
		bool					isCheck(int idPlayer);
		bool					isCheckMate();
		bool					isEquality();
		State					getGameState();
		
		void					computeValidMoves(int idPlayer);
		
		bool					play(int index);
		bool					play(std::string moveStr);
		bool					play(Move &move, bool checkValidity);
		bool					undo();

		void					movePiece(Move& move);
		void					movePiece(sf::Vector2i start, sf::Vector2i end);
		void					unMovePiece(Move &move);

		Piece*					getPiece(sf::Vector2i pos) const;
		Piece*					getPiece(int x, int y) const;

		void					addMove(sf::Vector2i start, sf::Vector2i end, std::vector<Move>& moves, bool checkConsidered, Tag tag = Tag::NoneTag);
		bool					isValidMove(Move move);
		bool					isCorrectMove(Move move) const;
		void					getMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		std::vector<Move>		getMoves(sf::Vector2i pos, bool checkConsidered);
		std::vector<Move>		getMoveOnPos(sf::Vector2i pos);
		Move					getSpecificMove(sf::Vector2i start, sf::Vector2i end);
		int						getIndexMove(sf::Vector2i start, sf::Vector2i end);
		std::vector<Move>		getAllMoves(int idPlayer, bool checkConsidered);
		int						getNumberMoves();
		bool					isAnyMovePossible(int idPlayer);

		std::pair<bool, float>	getEvaluationEndGame(int level);
		float					getEvaluation();

		std::string				getMoveSymbol(Move move);
		std::string				getMoveSymbol(int index);
		std::string				getStringToWright(int indexMove) const;
		std::string				getHeader() const;

		void					printMove(Move move);
		void					printValidMoves();
		void					printBoard() const;

		uint64_t				hashBoard() const;

	private :
		void					init();
		void					initText();
		void					initSprites();
		void					initFont();
		void					initHighlights();
		void					initializeZobristTable();

		void					drawBoard(sf::RenderWindow& target);
		void					setSpritePosition(sf::Sprite& sprite, sf::Vector2i pos) const;
		void					resetHighlights();

		bool					testMove(Move &move);

		float					eval();
		float					evalPieces() const;
		float					evalMoves();

		void					undo(Move &move);
		void					undo(Move &move, Move &previousMove);

		bool					isThreatenedBy(sf::Vector2i pos, int idPlayer) const;

		void					resetEnPassant(int idPlayer) const;
		void					getPawnMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		void					getKnightMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		void					getBishopMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		void					getTowerMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		void					getKingMoves(sf::Vector2i pos, std::vector<Move> &moves, bool checkConsidered);
		void					getDirectionMove(Piece* movingPiece, sf::Vector2i dir, std::vector<Move>& moves, bool checkConsidered);

		// Lecture ecriture
		std::string				generateBoardId() const;
		
	private :
		bool							reverseBoard;
		sf::Vector2i					selectedPawn;

		std::stack<short>				repetitiveMoves;

		Color							colorSquare[8][8];
		Color							baseColorSquare[8][8];

		Piece*							pieces[2][16];
		Piece*							piecesOnBoard[8][8];

		std::vector<Move>				history;
		std::vector<std::string>		historyBoard;
		std::stack< std::vector<Move>>  historyMoves;

		sf::RectangleShape				squareBoard;
		sf::Texture						boardTexture;
		sf::Texture						piecesTexture;

		std::map<Color, sf::Texture*>	highlights;
		std::map<Color, sf::Sprite*>	colorSprites;

		sf::Font*						font;

		sf::Sprite						boardSprite;
		sf::Sprite						pieceSprites[2][6];

		std::vector<sf::Text>			textsSquare;

		uint64_t						zobristTable[64][12];

		static bool						temp;
};
