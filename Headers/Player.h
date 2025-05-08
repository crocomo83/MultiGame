#pragma once

#include "BasicBoard.h"
#include "Piece.h"
#include "Node.h"
#include "NodePool.h"

class Player {

	public:
		enum PlayerType {
			Human,
			MinMax,
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
		static int							play(BasicBoard* board, PlayerType type, int idPlayer, int level);
		static std::pair<int, float>		playMinMaxSimple(BasicBoard* board, int level, int idPlayer);
		static float						playAlphaBeta(BasicBoard* board, int level, int idPlayer, bool root, float alpha, float beta);

		static bool							isHuman(PlayerType type);

	public:
		static NodePool pool;
		static int		nbTest;
		static int		nbCut;
};