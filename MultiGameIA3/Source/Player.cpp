#include "../Headers/Player.h"
#include "../Headers/Utility.h"

#include <iostream>
#include <cmath>
#include <chrono>

Player::Player(PlayerType type, int id_)
: myType(type)
, id(id_)
{

}

bool Player::isHuman() const {
	return myType == PlayerType::Human;
}

float Player::playRandom(Board* board, int idPlayer) {
	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int choice = rand() % moves.size();
	return choice;
}

std::pair<int, float> Player::playMinMax(Board* board, int level, int idPlayer) {
	if (level == 0) {
		float valueBoard = board->eval(idPlayer, 1, 0.1, 2);
		return std::pair<int, float>(-1, valueBoard);
	}
	else {
		Board::State state = board->getGameState();
		switch (state) {
			case Board::State::CheckMateWhite:
				return std::pair<int, float>(-1, -1000);
				break;
			case Board::State::CheckMateBlack:
				return std::pair<int, float>(-1, 1000);
				break;
			case Board::State::Equality:
				return std::pair<int, float>(-1, 0);
				break;
		}
	}

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int n = moves.size();

	if (n == 0) {
		std::cerr << "Pat ou checkmate aurait du etre detecte !" << std::endl;
		return std::pair<int, float>(-1, 0);
	}
	
	Move& firstMove = moves[0];
	int numBest = 0;
	float best;

	if (idPlayer == 0) {
		best = playMinMax(board, level - 1, 1).second;
		for (int i = 1; i < n; i++) {
			Move &move = moves[i];
			board->play(move, false);

			float value = playMinMax(board, level - 1, 1).second;

			if (value < best) {
				best = value;
				numBest = i;
			}

			board->undo();
		}
	}
	else {
		best = playMinMax(board, level - 1, 0).second;
		for (int i = 1; i < n; i++) {
			Move &move = moves[i];
			board->play(move, false);

			float value = playMinMax(board, level - 1, 0).second;

			if (value > best) {
				best = value;
				numBest = i;
			}

			board->undo();
		}
	}

	return std::pair<int, float>(numBest, best);
}

std::map<std::string, float> Player::getMinMaxAllMovesValue(Board* board, int level, int idPlayer) {
	std::map<std::string, float> valueMoves;

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int n = moves.size();

	std::vector<Move>::iterator it;
	for (it = moves.begin(); it != moves.end(); ++it) {
		Move move = *it;
		board->play(move, false);
		float value = playMinMax(board, level - 1, otherPlayer(idPlayer)).second;
		valueMoves.insert(std::pair<std::string, float>(board->getMoveSymbol(move), value));
		board->undo();
	}

	return valueMoves;
}

float Player::playAlphaBeta(Board* board, int level, int idPlayer, bool root, Move& lastMove, float alpha_, float beta_) {
	if (level == 0) {
		return board->eval(lastMove.player, 1, 0.1, 2);
	}

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int n = moves.size();

	if (n == 0) {
		return 0;
	}

	float alpha = alpha_;
	float beta = beta_;
	int numBest = -1;
	float best;
	
	if (idPlayer == 0) {
		best = 1000000;
		for (int i = 0; i < n; i++) {
			Move &move = moves[i];
			board->play(move, false);

			float value = playAlphaBeta(board, level - 1, 1, false, move, alpha, beta);

			if (value < best) {
				best = value;
				numBest = i;
			}

			board->undo();

			if (alpha >= best) {
				return best;
			}
			beta = std::min(beta, best);
		}
	}
	else {
		best = -1000000;
		for (int i = 0; i < n; i++) {
			Move &move = moves[i];
			board->play(move, false);

			float value = playAlphaBeta(board, level - 1, 0, false, move, alpha, beta);

			if (value > best) {
				best = value;
				numBest = i;
			}

			board->undo();

			if (best >= beta) {
				return best;
			}
			alpha = std::max(alpha, best);
		}
	}

	if (root) {
		if (numBest == -1) {
			std::cout << "Problem : " << idPlayer << std::endl;
			std::cout << moves.size() << std::endl;
		}
		return numBest;
	}
	else {
		return best;
	}
}

Move Player::play(Board* board) {
	Move move = Move();
	int index;

	auto start = std::chrono::high_resolution_clock::now();

	switch (myType) {
		case MinMax :
			std::cout << "Play : " << "MinMax" << std::endl;
			index = playMinMax(board, 3, id).first;
			break;
		case AlphaBeta :
			std::cout << "Play : " << "AlphaBeta" << std::endl;
			index = playAlphaBeta(board, 3, id, true, move, -1000000, 10000000);
			break;
		case Random:
			std::cout << "Play : " << "Random" << std::endl;
			index = playRandom(board, id);
			break;
	}
	auto end = std::chrono::high_resolution_clock::now();

	// Calcul du temps écoulé en millisecondes
	double duration = std::chrono::duration<double, std::milli>(end - start).count();

	std::cout << "Temps d'execution: " << duration << " ms" << std::endl;

	std::vector<Move> moves = board->getAllMoves(id, true);
	return moves[index];
}
