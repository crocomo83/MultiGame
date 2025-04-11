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

float Player::playRandom(Board* board, int idPlayer) {
	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int choice = rand() % moves.size();
	return choice;
}

std::pair<int, float> Player::playMinMaxSimple(Board* board, int level, int idPlayer) {
	nbTest++;
	Board::State state = board->getGameState();
	switch (state) {
	case Board::State::CheckMateWhite:
		return { -1, -1000 };
	case Board::State::CheckMateBlack:
		return { -1, 1000 };
	case Board::State::Equality:
		return { -1, 0 };
	}

	if (level == 0) {
		float valueBoard = board->getEvaluation();
		return { -1, valueBoard };
	}

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int n = moves.size();

	if (n == 0) {
		std::cerr << "Pat ou checkmate aurait dus etre detectes !" << std::endl;
		return { -1, 0 };
	}

	Move& firstMove = moves[0];
	int numBest = 0;
	float best;

	if (idPlayer == 0) {
		best = playMinMaxSimple(board, level - 1, 1).second;
		for (int i = 1; i < n; i++) {
			Move& move = moves[i];
			board->play(move, false);

			float value = playMinMaxSimple(board, level - 1, 1).second;

			if (value < best) {
				best = value;
				numBest = i;
			}

			board->undo();
		}
	}
	else {
		best = playMinMaxSimple(board, level - 1, 0).second;
		for (int i = 1; i < n; i++) {
			Move& move = moves[i];
			board->play(move, false);

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

void Player::playMinMax(Board* board, int level, int idPlayer, Node* parent) {
	Node::Mode mode = (idPlayer == 0 ? Node::Mode::Min : Node::Mode::Max);
	Board::State state = board->getGameState();
	switch (state) {
	case Board::State::CheckMateWhite:
		parent->value = -1000.0f;
		return;
	case Board::State::CheckMateBlack:
		parent->value = 1000.0f;
		return;
	case Board::State::Equality:
		parent->value = 0.0f;
		return;
	}

	if (level == 0) {
		parent->value = board->getEvaluation();
		return;
	}

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	int n = moves.size();

	if (n == 0) {
		std::cerr << "Pat ou checkmate aurait du etre detecte !" << std::endl;
		parent->addChild(pool.createNode(0.0f, mode));
		return;
	}

	for (int i = 0; i < n; i++) {
		board->play(moves[i], false);

		Node* child = pool.createNode(0.0f, mode);
		child->setMoveSymbol(board->getMoveSymbol(moves[i]));
		playMinMax(board, level - 1, otherPlayer(idPlayer), child);
		parent->addChild(child);

		board->undo();
	}
	parent->computeValue();
}

float Player::playAlphaBeta(Board* board, int level, int idPlayer, bool root, float alpha_, float beta_) {
	nbTest++;
	Board::State state = board->getGameState();
	switch (state) {
	case Board::State::CheckMateWhite:
		return -1000;
	case Board::State::CheckMateBlack:
		return 1000;
	case Board::State::Equality:
		return 0;
	}

	if (level == 0) {
		return board->getEvaluation();
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
			Move &move = moves[i];
			board->play(move, false);

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
			std::cout << moves.size() << std::endl;
		}
		return numBest;
	}
	else {
		return best;
	}
}

Move Player::play(Board* board, PlayerType type, int idPlayer) {
	Move move = Move();
	int index;
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
		index = playAlphaBeta(board, 3, idPlayer, true, -1000000, 10000000);
	}
	else if (type == PlayerType::Random) {
		std::cout << "Play : " << "Random" << std::endl;
		index = playRandom(board, idPlayer);
	}
	auto end = std::chrono::high_resolution_clock::now();

	// Calcul du temps écoulé en millisecondes
	double duration = std::chrono::duration<double, std::milli>(end - start).count();

	std::cout << "Temps d'execution: " << duration << " ms" << std::endl;
	std::cout << nbTest << " moves calcules" << std::endl;

	std::vector<Move> moves = board->getAllMoves(idPlayer, true);
	return moves[index];
}
