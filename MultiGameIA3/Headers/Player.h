#pragma once

#include "IBoard.h"
#include "Piece.h"
#include "Node.h"
#include "NodePool.h"

class Player {

	public:
		enum PlayerType {
			Human,
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
		static int							play(IBoard* board, PlayerType type, int idPlayer);
		static std::pair<int, float>		playMinMaxSimple(IBoard* board, int level, int idPlayer);
		static void							playMinMax(IBoard* board, int level, int idPlayer, Node* parent);
		static float						playAlphaBeta(IBoard* board, int level, int idPlayer, bool root, float alpha, float beta);

		static bool							isHuman(PlayerType type);

	public:
		static NodePool pool;
		static int		nbTest;
};