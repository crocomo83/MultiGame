#include "../Headers/Player.h"
#include "../Headers/Utility.h"

#include <iostream>
#include <cmath>
#include <chrono>

int Player::nbTest = 0;
int Player::nbCut = 0;

bool Player::isHuman(PlayerType type) {
	return type == PlayerType::Human;
}

std::pair<int, float> Player::playMinMaxSimple(IBoard* board, int level, int idPlayer) {
	nbTest++;

	std::pair<bool, float> resEndGame = board->getEvaluationEndGame(level);
	if (resEndGame.first) {
		return { -1, resEndGame.second };
	}

	if (level == 0) {
		float valueBoard = board->getEvaluation();
		return { -1, valueBoard };
	}

	int n = board->getNumberMoves();

	if (n == 0) {
		std::cerr << "Pat ou checkmate aurait dus etre detectes !" << std::endl;
		return { -1, 0 };
	}

	int numBest = -1;
	float best = (idPlayer == 0 ? 10000 : -10000);

	if (idPlayer == 0) {
		for (int i = 0; i < n; i++) {
			board->play(i);

			float value = playMinMaxSimple(board, level - 1, 1).second;

			if (value < best) {
				best = value;
				numBest = i;
			}

			board->undo();
		}
	}
	else {
		for (int i = 0; i < n; i++) {
			board->play(i);

			float value = playMinMaxSimple(board, level - 1, 0).second;

			if (value > best) {
				best = value;
				numBest = i;
			}

			board->undo();
		}
	}
	return { numBest, best };
}

float Player::playAlphaBeta(IBoard* board, int level, int idPlayer, bool root, float alpha_, float beta_) {
	nbTest++;

	std::pair<bool, float> resEndGame = board->getEvaluationEndGame(level);
	if (resEndGame.first) {
		return resEndGame.second;
	}

	if (level == 0) {
		return board->getEvaluation();
	}

	int n = board->getNumberMoves();

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
			board->play(i);
			float value = playAlphaBeta(board, level - 1, 1, false, alpha, beta);
			board->undo();

			if (value < best) {
				best = value;
				numBest = i;
				if (best <= alpha) {
					nbCut += (n - 1 - i);
					return best;
				}
				beta = std::min(beta, best);
			}
		}
	}
	else {
		best = -1000000;
		for (int i = 0; i < n; i++) {
			board->play(i);
			float value = playAlphaBeta(board, level - 1, 0, false, alpha, beta);
			board->undo();

			if (value > best) {
				best = value;
				numBest = i;
				if (best >= beta) {
					nbCut += (n - 1 - i);
					return best;
				}
				alpha = std::max(alpha, best);
			}
		}
	}

	return (root ? numBest : best);
}

int Player::play(IBoard* board, PlayerType type, int idPlayer, int level) {
	if (level == -1) {
		return -1;
	}

	int index = -1;
	nbTest = 0;
	nbCut = 0;

	auto start = std::chrono::high_resolution_clock::now();

	std::cout << "coups possibles : " << board->getNumberMoves() << std::endl;

	if (type == PlayerType::MinMax) {
		std::cout << "Play : " << "MinMax" << std::endl;
		index = playMinMaxSimple(board, level, idPlayer).first;
		std::cout << "Moves calcules : " << nbTest << std::endl;
	}
	else if (type == PlayerType::AlphaBeta) {
		std::cout << "Play : " << "AlphaBeta" << std::endl;
		index = playAlphaBeta(board, level, idPlayer, true, -1000000, 10000000);
		float percentCut = (float)nbCut / (float)(nbCut + nbTest) * 100.0f;
		std::cout << "Moves calcules : " << nbTest << " / " << (nbTest + nbCut) << ", percent cut : " << percentCut << "%" << std::endl;
	}
	auto end = std::chrono::high_resolution_clock::now();

	// Calcul du temps écoulé en millisecondes
	double duration = std::chrono::duration<double, std::milli>(end - start).count();

	std::cout << "Temps d'execution: " << duration << " ms" << std::endl;

	std::cout << "play : " << index << std::endl;
	return index;
}
