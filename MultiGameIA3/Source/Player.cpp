#include "../Headers/Player.h"
#include "../Headers/Utility.h"

#include <iostream>
#include <cmath>
#include <chrono>

NodePool Player::pool;
int Player::nbTest = 0;

void Player::init() {
	size_t maxNodes = 1000000;  // Capacité du pool (1 million de nœuds)
	pool = NodePool(maxNodes);
}

bool Player::isHuman(PlayerType type) {
	return type == PlayerType::Human;
}

std::pair<int, float> Player::playMinMaxSimple(IBoard* board, int level, int idPlayer) {
	nbTest++;

	std::pair<bool, float> resEndGame = board->getEvaluationEndGame();
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
	float best = (idPlayer == 0 ? 1000 : -1000);

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

void Player::playMinMax(IBoard* board, int level, int idPlayer, Node* parent) {
	Node::Mode mode = (idPlayer == 0 ? Node::Mode::Min : Node::Mode::Max);

	std::pair<bool, float> resEndGame = board->getEvaluationEndGame();
	if (resEndGame.first) {
		parent->value = resEndGame.second;
		return;
	}

	if (level == 0) {
		parent->value = board->getEvaluation();
		return;
	}

	int n = board->getNumberMoves();

	if (n == 0) {
		std::cerr << "Pat ou checkmate aurait du etre detecte !" << std::endl;
		parent->addChild(pool.createNode(0.0f, mode));
		return;
	}

	for (int i = 0; i < n; i++) {
		board->play(i);

		Node* child = pool.createNode(0.0f, mode);
		child->setMoveSymbol(board->getMoveSymbol(i));
		playMinMax(board, level - 1, otherPlayer(idPlayer), child);
		parent->addChild(child);

		board->undo();
	}
	parent->computeValue();
}

float Player::playAlphaBeta(IBoard* board, int level, int idPlayer, bool root, float alpha_, float beta_) {
	nbTest++;

	std::pair<bool, float> resEndGame = board->getEvaluationEndGame();
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
			board->play(i);

			float value = playAlphaBeta(board, level - 1, 0, false, alpha, beta);

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
		}
		return numBest;
	}
	else {
		return best;
	}
}

int Player::play(IBoard* board, PlayerType type, int idPlayer) {
	Move move = Move();
	int index = -1;
	nbTest = 0;

	auto start = std::chrono::high_resolution_clock::now();

	if (type == PlayerType::MinMax) {
		std::cout << "Play : " << "MinMax" << std::endl;
		index = playMinMaxSimple(board, 4, idPlayer).first;
	}
	else if (type == PlayerType::MinMaxDebug) {
		std::cout << "Play : " << "MinMaxDebug" << std::endl;
		Node::Mode mode = (idPlayer == 0 ? Node::Mode::Min : Node::Mode::Max);
		Node* root = pool.createNode(0.0f, mode);
		playMinMax(board, 3, idPlayer, root);
		index = root->getBestChild();
	}
	else if (type == PlayerType::AlphaBeta) {
		std::cout << "Play : " << "AlphaBeta" << std::endl;
		index = playAlphaBeta(board, 4, idPlayer, true, -1000000, 10000000);
	}
	auto end = std::chrono::high_resolution_clock::now();

	// Calcul du temps écoulé en millisecondes
	double duration = std::chrono::duration<double, std::milli>(end - start).count();

	std::cout << "Temps d'execution: " << duration << " ms" << std::endl;
	std::cout << nbTest << " moves calcules" << std::endl;

	return index;
}
