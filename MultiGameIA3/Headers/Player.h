#pragma once

#include "Board.h"
#include "Piece.h"

class Player {

	public:
		enum PlayerType {
			Human,
			Random,
			MinMax,
			AlphaBeta
		};

		struct Config {
			float coeffPieces;
			float coeffMoves;
		};

	public :
										Player(PlayerType type, int id_);
		Move							play(Board* board);
		float							playRandom(Board* board, int idPlayer);
		std::pair<int, float>			playMinMax(Board* board, int level, int idPlayer);
		std::map<std::string, float>	getMinMaxAllMovesValue(Board* board, int level, int idPlayer);
		float							playAlphaBeta(Board* board, int level, int idPlayer, bool root, Move& lastMove, float alpha, float beta);

		bool							isHuman() const;

	private : 
		PlayerType		myType;
		int				id;
};
