#pragma once

#include <queue>
#include <map>
#include <unordered_map>
#include <vector>
#include <stack>
#include <array>

#include "Piece.h"
#include "Utility.h"
#include "BasicBoard.h"
#include "Graphics/IDrawable.h"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class ChessBoard : public BasicBoard {
	public : 
								ChessBoard(bool reverseBoard_);

		// ===== IDrawable methods =====
		void					update(sf::Vector2i mousePosition) override;
		void					render(sf::RenderWindow& window) override;
		int						handleEvent(const std::optional<sf::Event> event) override;
		void					reset() override;

		// ===== BasicBoard methods =====
		bool					isGameOver() override;

		void					printValidMoves() override;
		bool					play(int index) override;
		bool					play(std::string moveStr) override;
		bool					undo() override;

		size_t					getNumberMoves() override;
		std::string				getStringToWright(int indexMove) const override;
		std::string				getHeader() const override;
		std::pair<bool, float>	getEvaluationEndGame(int level) override;
		float					getEvaluation() override;
		std::string				getMoveSymbol(int index) override;
		std::string				getPlayerName(int player) const override;
		BasicBoard::State		getGameState() const override;
		
		void					computeGameState() override;

		uint64_t				hashBoard() const override;
		void					printBoard() const override;

		// ===== ChessBoard methods =====
		void					select(sf::Vector2i mousePos);
		int						unselect(sf::Vector2i mousePos);

		// Test de situations
		bool					isCheck(int idPlayer) const;
		bool					isCheckMate();
		bool					isEquality();
		
		void					computeValidMoves(int idPlayer);
		
		bool					play(Move &move, bool checkValidity);

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
		bool					isAnyMovePossible(int idPlayer);

		std::string				getMoveSymbol(Move move);

		void					printMove(Move move);

	private :
		void					init();
		void					initText();
		void					initSprites();
		void					initFont();
		void					initHighlights();
		void					initializeZobristTable();

		void					drawBoard(sf::RenderWindow& target);
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

		sf::RectangleShape				colorRectangles[8][8];

		Piece*							pieces[2][16];
		Piece*							piecesOnBoard[8][8];

		std::vector<Move>				history;
		std::vector<std::string>		historyBoard;
		std::stack< std::vector<Move>>  historyMoves;
		std::vector<State>				historyState;

		sf::RectangleShape				squareBoard;
		sf::Texture						boardTexture;
		sf::Texture						piecesTexture;

		sf::Font*						font;

		std::array<sf::Sprite*, 12>		pieceSprites;

		std::vector<sf::Text>			textsSquare;

		uint64_t						zobristTable[64][12];

		static bool						temp;
};
