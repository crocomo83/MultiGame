#pragma once

#include "Board.h"
#include "Piece.h"
#include "Node.h"
#include "NodePool.h"

class Player {

	public:
		enum PlayerType {
			Human,
			Random,
			MinMax,
			MinMaxDebug,
			AlphaBeta
		};

		struct Data {
			PlayerType type;
			int id;
		};

		struct Config {
			float coeffPieces;
			float coeffMoves;
		};

	public :
											Player() = delete;
		static void							init();
		static Move							play(Board* board, PlayerType type, int idPlayer);
		static float						playRandom(Board* board, int idPlayer);
		static std::pair<int, float>		playMinMaxSimple(Board* board, int level, int idPlayer);
		static void							playMinMax(Board* board, int level, int idPlayer, Node* parent);
		static float						playAlphaBeta(Board* board, int level, int idPlayer, bool root, float alpha, float beta);

		static bool							isHuman(PlayerType type);

	public:
		static NodePool pool;
		static int		nbTest;
};