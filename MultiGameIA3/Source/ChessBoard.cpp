#include "../Headers/ChessBoard.h"

#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <direct.h>
#include <random>
#include <SFML/Graphics.hpp>

const float startX = 30;
const float startY = 30;
const float offsetX = 60;
const float offsetY = 60;

const float weightPieces = 1;
const float weightMoves = 0.2f;
const float weightRandom = 0;

bool ChessBoard::temp = false;

ChessBoard::ChessBoard(bool reverseBoard_)
	: repetitiveMoves()
	, history()
	, historyBoard()
	, historyMoves()
	, reverseBoard(reverseBoard_)
{
	idCurrentPlayer = 0;
	gameOver = false;
	selectedPawn = sf::Vector2i(-1, -1);
	mousePos = sf::Vector2i(-1, -1);

	repetitiveMoves.push(0);

	init();
	initFont();
	initText();
	initSprites();
	initHighlights();
	initializeZobristTable();

	history.reserve(1000);
	historyBoard.reserve(1000);
	computeValidMoves(0);
}

void ChessBoard::init() {
	for (int i = 0; i < 8; i++) {
		pieces[0][i] = new Piece(Pawn, 0, sf::Vector2i(i, 1));
		pieces[1][i] = new Piece(Pawn, 1, sf::Vector2i(i, 6));
	}

	pieces[0][8] = new Piece(Tower, 0, sf::Vector2i(0, 0));
	pieces[0][9] = new Piece(Knight, 0, sf::Vector2i(1, 0));
	pieces[0][10] = new Piece(Bishop, 0, sf::Vector2i(2, 0));
	pieces[0][11] = new Piece(Queen, 0, sf::Vector2i(3, 0));
	pieces[0][12] = new Piece(King, 0, sf::Vector2i(4, 0));
	pieces[0][13] = new Piece(Bishop, 0, sf::Vector2i(5, 0));
	pieces[0][14] = new Piece(Knight, 0, sf::Vector2i(6, 0));
	pieces[0][15] = new Piece(Tower, 0, sf::Vector2i(7, 0));

	pieces[1][8] = new Piece(Tower, 1, sf::Vector2i(0, 7));
	pieces[1][9] = new Piece(Knight, 1, sf::Vector2i(1, 7));
	pieces[1][10] = new Piece(Bishop, 1, sf::Vector2i(2, 7));
	pieces[1][11] = new Piece(Queen, 1, sf::Vector2i(3, 7));
	pieces[1][12] = new Piece(King, 1, sf::Vector2i(4, 7));
	pieces[1][13] = new Piece(Bishop, 1, sf::Vector2i(5, 7));
	pieces[1][14] = new Piece(Knight, 1, sf::Vector2i(6, 7));
	pieces[1][15] = new Piece(Tower, 1, sf::Vector2i(7, 7));

	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			piecesOnBoard[i][j] = nullptr;
		}
	}

	for (int idPlayer = 0; idPlayer < 2; idPlayer++) {
		for (int i = 0; i < 16; i++) {
			Piece* piece = pieces[idPlayer][i];
			sf::Vector2i pos = piece->pos;
			piecesOnBoard[pos.x][pos.y] = piece;
		}
	}

	historyBoard.push_back(generateBoardId());
}

void ChessBoard::initText() {
	sf::Vector2f startTextNum(7, 40);
	for (int i = 1; i < 9; i++) {
		sf::Text text;
		text.setFont(*font);
		text.setString(std::to_string(i));
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f(startTextNum) + sf::Vector2f(0, (8 - i) * offsetY));

		textsSquare.push_back(text);
	}

	sf::Vector2f startTextAlph(52, 508);
	std::string bottomText = "abcdefgh";
	for (int i = 0; i < 8; i++) {
		sf::Text text;
		text.setFont(*font);
		text.setString(bottomText[i]);
		text.setCharacterSize(25);
		text.setFillColor(sf::Color::Black);
		text.setPosition(sf::Vector2f(startTextAlph) + sf::Vector2f(i * offsetX, 0));

		textsSquare.push_back(text);
	}
}

void ChessBoard::initSprites() {

	if (piecesTexture.loadFromFile("Media/Pieces_small_size.png"))
	{
		pieceSprites[1][0] = sf::Sprite(piecesTexture, sf::IntRect(14, 22, 61, 61));
		pieceSprites[1][1] = sf::Sprite(piecesTexture, sf::IntRect(83, 22, 61, 61));
		pieceSprites[1][2] = sf::Sprite(piecesTexture, sf::IntRect(152, 20, 61, 61));
		pieceSprites[1][3] = sf::Sprite(piecesTexture, sf::IntRect(217, 20, 61, 61));
		pieceSprites[1][4] = sf::Sprite(piecesTexture, sf::IntRect(278, 22, 61, 61));
		pieceSprites[1][5] = sf::Sprite(piecesTexture, sf::IntRect(332, 24, 61, 61));

		pieceSprites[0][0] = sf::Sprite(piecesTexture, sf::IntRect(14, 95, 61, 61));
		pieceSprites[0][1] = sf::Sprite(piecesTexture, sf::IntRect(83, 95, 61, 61));
		pieceSprites[0][2] = sf::Sprite(piecesTexture, sf::IntRect(152, 93, 61, 61));
		pieceSprites[0][3] = sf::Sprite(piecesTexture, sf::IntRect(217, 93, 61, 61));
		pieceSprites[0][4] = sf::Sprite(piecesTexture, sf::IntRect(278, 95, 61, 61));
		pieceSprites[0][5] = sf::Sprite(piecesTexture, sf::IntRect(332, 97, 61, 61));
	}
	else {
		std::cerr << "Error loading pieces" << std::endl;
	}

	squareBoard.setSize(sf::Vector2f(8 * offsetX, 8 * offsetY));
	squareBoard.setPosition(startX, startY);
	squareBoard.setFillColor(sf::Color::Transparent);
	squareBoard.setOutlineColor(sf::Color::Black);
	squareBoard.setOutlineThickness(3);
}

void ChessBoard::initFont() {
	font = new sf::Font();
	if (!font->loadFromFile("arial.ttf"))
	{
		std::cout << "error while loading font" << std::endl;
	}
}

void ChessBoard::initHighlights() {
	std::map<Color, std::string> pathColor;
	pathColor[Color::Blue] = "Media/blue.png";
	pathColor[Color::Green] = "Media/green.png";
	pathColor[Color::Red] = "Media/red.png";
	pathColor[Color::White] = "Media/white.png";
	pathColor[Color::Black] = "Media/black.png";

	for (auto itr = pathColor.begin(); itr != pathColor.end(); ++itr) {
		Color color = itr->first;
		std::string path = itr->second;
		highlights[color] = new sf::Texture();
		if (highlights[color]->loadFromFile(path)) {
			colorSprites[color] = new sf::Sprite(*(highlights[color]), sf::IntRect(0, 0, offsetX, offsetY));
		}
	}

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if ((x + y) % 2 == 0) {
				baseColorSquare[x][y] = Color::White;
				colorSquare[x][y] = Color::White;
			}
			else {
				baseColorSquare[x][y] = Color::Black;
				colorSquare[x][y] = Color::Black;
			}
		}
	}
}

void ChessBoard::resetHighlights() {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			colorSquare[x][y] = baseColorSquare[x][y];
		}
	}
}

void ChessBoard::initializeZobristTable() {
	std::mt19937_64 rng(42);
	std::uniform_int_distribution<uint64_t> dist;

	for (int square = 0; square < 64; ++square) {
		for (int piece = 0; piece < 12; ++piece) {
			zobristTable[square][piece] = dist(rng);
		}
	}
}

bool ChessBoard::isGameOver(std::string& messageGameOver) {
	ChessBoard::State state = getGameState();
	switch (state) {
	case ChessBoard::State::CheckMateWhite:
		messageGameOver = "Check Mate, White win!";
		return true;
	case ChessBoard::State::CheckMateBlack:
		messageGameOver = "Check Mate, Black win!";
		return true;
	case ChessBoard::State::Equality:
		messageGameOver = "Equality";
		return true;
	default:
		return false;
	}
}

void ChessBoard::handleEvent(sf::Vector2i mousePos, sf::Event& event) {
	switch (event.type) {
	case sf::Event::MouseButtonPressed:
		if (event.mouseButton.button == sf::Mouse::Left) {
			select(mousePos);
		}
		break;
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			unselect(mousePos);
		}
		break;
	}
}

bool ChessBoard::isValidMove(Move move) {
	std::vector<Move> validMoves = historyMoves.top();
	for (int i = 0; i < validMoves.size(); i++) {
		if (equalMoves(move, validMoves[i])) {
			return true;
		}
	}
	return false;
}

bool ChessBoard::isCorrectMove(Move move) const {
	if (!isOnBoard(move.end.x, move.end.y) || !isOnBoard(move.begin.x, move.begin.y)) {
		return false;
	}

	Piece* target = getPiece(move.end);
	if (target != nullptr && target->player == move.player) {
		return false;
	}
	return true;
}

void ChessBoard::select(sf::Vector2i mousePos) {
	if (selectedPawn.x == -1) {
		sf::Vector2i posOnBoard = PixelToChessBoard(mousePos, sf::Vector2i(startX, startY), sf::Vector2i(offsetX, offsetY), reverseBoard);

		if (!isOnBoard(posOnBoard)) {
			return;
		}

		Piece* piece = getPiece(posOnBoard);
		if (piece == nullptr || piece->player != idCurrentPlayer) {
			return;
		}

		selectedPawn = posOnBoard;
	}
}

bool ChessBoard::unselect(sf::Vector2i mousePos) {
	bool played = false;
	if (selectedPawn.x != -1) {
		sf::Vector2i posOnBoard = PixelToChessBoard(mousePos, sf::Vector2i(startX, startY), sf::Vector2i(offsetX, offsetY), reverseBoard);

		Move move = getSpecificMove(selectedPawn, posOnBoard);

		std::string symbol = getMoveSymbol(move);

		played = play(move, true);

		if (played) {
			std::cout << (move.player == 0 ? "white" : "black") << " play " << symbol << std::endl;
			std::cout << std::endl;
		}

		resetHighlights();

		selectedPawn = sf::Vector2i(-1, -1);
	}
	return played;
}

std::vector<Move> ChessBoard::getAllMoves(int idPlayer, bool checkConsidered) {
	std::vector<Move> moves;
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		if (!piece->taken) {
			getMoves(piece->pos, moves, checkConsidered);
		}
	}
	return moves;
}

int ChessBoard::getNumberMoves() {
	std::vector<Move> validMoves = historyMoves.top();
	return validMoves.size();
}

bool ChessBoard::isAnyMovePossible(int idPlayer) {
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		if (!piece->taken) {
			std::vector<Move> moves = getMoves(piece->pos, true);
			if (!moves.empty()) {
				return true;
			}
		}
	}
	return false;
}

void ChessBoard::computeValidMoves(int idPlayer) {
	std::vector<Move> validMoves;
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		if (!piece->taken) {
			getMoves(piece->pos, validMoves, true);
		}
	}
	historyMoves.push(validMoves);
}

bool ChessBoard::play(Move& move, bool checkValidity) {
	if (checkValidity) {
		if (!isValidMove(move)) {
			return false;
		}
	}

	resetEnPassant(move.player);

	int beginX = move.begin.x;
	int beginY = move.begin.y;
	int endX = move.end.x;
	int endY = move.end.y;

	Piece* piece = getPiece(move.begin);

	if (piece == nullptr) {
		std::cerr << "Erreur play : piece ne peut pas etre nulle" << std::endl;
		printBoard();
		return false;
	}

	switch (move.tag) {
	case EnPassant:
		move.destroyed = getPiece(endX, beginY);
		move.destroyed->taken = true;
		piecesOnBoard[endX][beginY] = nullptr;
		break;
	case JumpTwoCases:
		piece->hasJustMoveTwoCases = true;
		break;
	case Promotion:
		std::cout << "play promotion" << std::endl;
		piece->type = Queen;
		break;
	case KingSideCastling:
		movePiece(sf::Vector2i(7, beginY), sf::Vector2i(5, beginY));
		break;
	case QueenSideCastling:
		movePiece(sf::Vector2i(0, beginY), sf::Vector2i(3, beginY));
		break;
	}

	// Actual move
	movePiece(move);

	if (piece->type == Pawn || move.destroyed != nullptr) {
		repetitiveMoves.push(0);
	}
	else {
		repetitiveMoves.top()++;
	}

	idCurrentPlayer = otherPlayer(idCurrentPlayer);

	history.push_back(move);
	historyBoard.push_back(generateBoardId());
	computeValidMoves(idCurrentPlayer);

	return true;
}

bool ChessBoard::play(int index) {
	std::vector<Move> validMoves = historyMoves.top();
	return play(validMoves[index], false);
}

bool ChessBoard::undo() {
	int n = history.size();
	if (n == 0) {
		std::cout << "Can't undo : no move were made" << std::endl;
		return false;
	}

	Move& move = history[n - 1];
	if (n >= 2) {
		Move& previousMove = history[n - 2];
		undo(move, previousMove);
	}
	else {
		undo(move);
	}

	history.pop_back();
	historyBoard.pop_back();
	historyMoves.pop();

	idCurrentPlayer = otherPlayer(idCurrentPlayer);

	return true;
}

void ChessBoard::undo(Move& move) {
	int beginX = move.end.x;
	int beginY = move.end.y;
	int endX = move.begin.x;
	int endY = move.begin.y;

	Piece* piece = getPiece(move.end);

	Move* towerMove;
	switch (move.tag) {
	case NoneTag:
		break;
	case JumpTwoCases:
		piece->hasJustMoveTwoCases = false;
		break;
	case EnPassant:
		piecesOnBoard[move.destroyed->pos.x][move.destroyed->pos.y] = move.destroyed;
		move.destroyed->taken = false;
		break;
	case Promotion:
		piece->type = Pawn;
		break;
	case KingSideCastling:
		towerMove = new Move(getPiece(5, beginY), sf::Vector2i(7, beginY), sf::Vector2i(5, beginY));
		towerMove->destroyed = nullptr;
		unMovePiece(*towerMove);
		break;
	case QueenSideCastling:
		towerMove = new Move(getPiece(3, beginY), sf::Vector2i(0, beginY), sf::Vector2i(3, beginY));
		towerMove->destroyed = nullptr;
		unMovePiece(*towerMove);
		break;
	}

	// Actual move
	unMovePiece(move);

	if (repetitiveMoves.top() == 0) {
		repetitiveMoves.pop();
	}
	else {
		repetitiveMoves.top()--;
	}
}

void ChessBoard::undo(Move& move, Move& previousMove) {
	undo(move);

	// Pawn moved 2 cases
	if (previousMove.tag == JumpTwoCases) {
		piecesOnBoard[previousMove.end.x][previousMove.end.y]->hasJustMoveTwoCases = true;
	}
}

void ChessBoard::resetEnPassant(int idPlayer) const {
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		piece->hasJustMoveTwoCases = false;
	}
}

void ChessBoard::movePiece(sf::Vector2i start, sf::Vector2i end) {
	Move move(getPiece(start), end);
	movePiece(move);
}

void ChessBoard::movePiece(Move& move) {
	if (move.tag != Tag::EnPassant) {
		move.destroyed = getPiece(move.end);
		if (move.destroyed != nullptr) {
			move.destroyed->taken = true;
		}
	}
	Piece* piece = getPiece(move.begin);
	piece->pos = move.end;
	piece->nbMove++;
	piecesOnBoard[move.end.x][move.end.y] = piece;
	piecesOnBoard[move.begin.x][move.begin.y] = nullptr;
}

void ChessBoard::unMovePiece(Move& move) {
	Piece* piece = getPiece(move.end.x, move.end.y);
	piece->pos = move.begin;
	piece->nbMove--;
	piecesOnBoard[move.begin.x][move.begin.y] = piece;
	if (move.tag != Tag::EnPassant) {
		piecesOnBoard[move.end.x][move.end.y] = move.destroyed;
		if (move.destroyed != nullptr) {
			move.destroyed->taken = false;
		}
	}
	else {
		piecesOnBoard[move.end.x][move.end.y] = nullptr;
	}
}

void ChessBoard::update(sf::Vector2i pos, int idPlayer) {
	mousePos = pos;
	resetHighlights();

	// red highlight
	if (history.size() > 0) {
		Move& lastMove = history[history.size() - 1];
		colorSquare[lastMove.begin.x][lastMove.begin.y] = Color::Red;
		colorSquare[lastMove.end.x][lastMove.end.y] = Color::Red;
	}

	if (selectedPawn.x != -1) {
		// blue highlight
		std::vector<Move> moves = getMoveOnPos(selectedPawn);
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves[i];
			colorSquare[move.end.x][move.end.y] = Color::Blue;
		}

		sf::Vector2i posOnBoard = PixelToChessBoard(pos, sf::Vector2i(startX, startY), sf::Vector2i(offsetX, offsetY), reverseBoard);
		Move move = Move(getPiece(selectedPawn), posOnBoard);

		// green highlight
		if (isValidMove(move)) {
			colorSquare[posOnBoard.x][posOnBoard.y] = Color::Green;
		}
	}
}

void ChessBoard::drawBoard(sf::RenderWindow& target) {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Color color = colorSquare[x][y];
			sf::Sprite* sprite = colorSprites[color];
			setSpritePosition(*sprite, sf::Vector2i(x, y));
			target.draw(*sprite);
		}
	}

	target.draw(squareBoard);

	for (auto& text : textsSquare) {
		target.draw(text);
	}
}

void ChessBoard::draw(sf::RenderWindow& target) {
	drawBoard(target);

	for (int idPlayer = 0; idPlayer < 2; idPlayer++) {
		for (int i = 0; i < 16; i++) {
			Piece* piece = pieces[idPlayer][i];
			if (piece->pos != selectedPawn && !piece->taken) {
				setSpritePosition(pieceSprites[idPlayer][piece->type], piece->pos);
				target.draw(pieceSprites[idPlayer][piece->type]);
			}
		}
	}

	if (selectedPawn.x != -1) {
		Piece* selectedPiece = getPiece(selectedPawn);
		if (selectedPiece == nullptr) {
			std::cerr << "Erreur: selectedPiece est NULL!" << std::endl;
		}
		else {
			sf::Sprite* sprite = &pieceSprites[selectedPiece->player][selectedPiece->type];
			sprite->setPosition(mousePos.x - sprite->getLocalBounds().width / 2, mousePos.y - sprite->getLocalBounds().height / 2);
			target.draw(*sprite);
		}
	}
}

void ChessBoard::setSpritePosition(sf::Sprite& sprite, sf::Vector2i pos) const {

	sf::Vector2i posOnBoard = CoordToPixelChessBoard(pos, sf::Vector2i(startX, startY), sf::Vector2i(offsetX, offsetY), reverseBoard);
	sprite.setPosition(posOnBoard.x, posOnBoard.y);
}

bool ChessBoard::isThreatenedBy(sf::Vector2i pos, int idPlayer) const {
	// On cherche une tour ou une dame ou un roi qui menacent la case
	sf::Vector2i directionsLine[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	for (auto& dir : directionsLine) {
		sf::Vector2i currentPos = pos + dir;
		int nbIter = 0;
		while (isOnBoard(currentPos)) {
			nbIter++;
			Piece* piece = getPiece(currentPos);
			if (piece != nullptr) {
				if (piece->player == idPlayer) {
					if (piece->type == Tower || piece->type == Queen || (piece->type == King && nbIter == 1)) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			currentPos += dir;
		}
	}

	// On cherche un fou ou une dame ou un roi qui menacent la case
	sf::Vector2i directionsDiagonal[4] = { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} };

	for (auto& dir : directionsDiagonal) {
		sf::Vector2i currentPos = pos + dir;
		int nbIter = 0;
		while (isOnBoard(currentPos)) {
			nbIter++;
			Piece* piece = getPiece(currentPos);
			if (piece != nullptr) {
				if (piece->player == idPlayer) {
					if (piece->type == Bishop || piece->type == Queen || (piece->type == King && nbIter == 1)) {
						return true;
					}
					else {
						break;
					}
				}
				else {
					break;
				}
			}
			currentPos += dir;
		}
	}

	// On cherche un cavalier qui menace la case
	sf::Vector2i* possibilities = getKnightEmplacement(pos);

	for (int i = 0; i < 8; i++) {
		sf::Vector2i possibility = possibilities[i];
		Piece* piece = getPiece(possibility);
		if (piece != nullptr && piece->type == Knight && piece->player == idPlayer) {
			return true;
		}
	}

	// On cherche un pion qui menace la case
	// Sens d'avancee des pions
	int sign = (idPlayer == 0 ? -1 : 1);

	sf::Vector2i leftPos = pos + sf::Vector2i(-1, sign);
	Piece* leftPawn = getPiece(leftPos);
	if (leftPawn != nullptr && leftPawn->type == Pawn && leftPawn->player == idPlayer) {
		return true;
	}

	sf::Vector2i rightPos = pos + sf::Vector2i(1, sign);
	Piece* rightPawn = getPiece(rightPos);
	if (rightPawn != nullptr && rightPawn->type == Pawn && rightPawn->player == idPlayer) {
		return true;
	}

	return false;
}

bool ChessBoard::isCheck(int idPlayer) {
	Piece* king = pieces[otherPlayer(idPlayer)][12];
	return isThreatenedBy(king->pos, idPlayer);
}

ChessBoard::State ChessBoard::getGameState() {
	// Debut de partie
	if (history.empty()) {
		return ChessBoard::State::Normal;
	}

	Move& lastMove = history[history.size() - 1];
	int idPlayer = lastMove.player;

	// Evite d'appeler ces fonction deux fois : pour checkmate et pat
	bool check = isCheck(idPlayer);
	bool opponentCanPlay = isAnyMovePossible(otherPlayer(idPlayer));

	if (check && !opponentCanPlay) {
		if (idPlayer == 0) {
			return ChessBoard::State::CheckMateWhite;
		}
		else {
			return ChessBoard::State::CheckMateBlack;
		}
	}
	else if (!check && !opponentCanPlay) {
		return ChessBoard::State::Equality;
	}
	else if (isEquality()) {
		return ChessBoard::State::Equality;
	}
	return ChessBoard::State::Normal;
}

std::pair<bool, float> ChessBoard::getEvaluationEndGame(int level) {
	State state = getGameState();
	switch (state) {
	case(Normal):
		return { false, -1 };
	case(CheckMateWhite):
		return { true, -1000 };
	case(CheckMateBlack):
		return { true, 1000 };
	case(Equality):
		return { true, 0 };
	}
}

bool ChessBoard::isCheckMate() {
	// Debut de partie => pas d'echec et mat
	if (history.empty()) {
		return false;
	}

	// Pat
	Move& lastMove = history[history.size() - 1];
	return isCheck(lastMove.player) && !isAnyMovePossible(otherPlayer(lastMove.player));
}

bool ChessBoard::isEquality() {
	// Debut de partie => pas d'égalité
	if (history.empty()) {
		return false;
	}

	// position répétée 3 fois => égalité
	std::string currentBoard = historyBoard[historyBoard.size() - 1];
	int nbIdenticalBoard = std::count(historyBoard.begin(), historyBoard.end(), currentBoard);
	if (nbIdenticalBoard == 3) {
		return true;
	}
	else if (nbIdenticalBoard > 3) {
		std::cerr << nbIdenticalBoard << " board identiques ont existés, une égalité aurait due être déclarée !" << std::endl;
		return true;
	}

	// 50 moves sans prise ou mouvement de pions => égalité
	if (repetitiveMoves.top() >= 50) {
		return true;
	}
	return false;
}

float ChessBoard::getEvaluation() {
	uint64_t hash = hashBoard();
	auto it = zobristCache.find(hash);
	if (it != zobristCache.end()) {
		return it->second;
	}

	float value = eval();
	zobristCache[hash] = value;
	return value;
}

float ChessBoard::eval() {
	float score = 0;
	score += weightPieces * evalPieces();
	score += weightMoves * evalMoves();
	score += weightRandom * (2 * (float)rand() / (float)RAND_MAX - 1);
	return score;
}

float ChessBoard::evalPieces() const {
	float total = 0;
	for (int idPlayer = 0; idPlayer < 2; idPlayer++) {
		for (int i = 0; i < 16; i++) {
			Piece* piece = pieces[idPlayer][i];
			if (!piece->taken) {
				float rawValue = 0;
				switch (piece->type) {
				case Pawn:
					rawValue = 1;
					break;
				case Bishop:
					rawValue = 3;
					break;
				case Knight:
					rawValue = 3;
					break;
				case Tower:
					rawValue = 5;
					break;
				case Queen:
					rawValue = 9;
					break;
				}
				if (piece->player == 0) {
					total -= rawValue;
				}
				else {
					total += rawValue;
				}
			}
		}
	}
	return total;
}

float ChessBoard::evalMoves() {
	std::vector<Move> moves0 = getAllMoves(0, false);
	std::vector<Move> moves1 = getAllMoves(1, false);
	int diff = moves1.size() - moves0.size();
	return (float)diff;
}

Piece* ChessBoard::getPiece(sf::Vector2i pos) const {
	return getPiece(pos.x, pos.y);
}

Piece* ChessBoard::getPiece(int x, int y) const {
	if (x < 0 || x > 7 || y < 0 || y > 7) {
		return nullptr;
	}
	return piecesOnBoard[x][y];
}

std::vector<Move> ChessBoard::getMoves(sf::Vector2i pos, bool checkConsidered) {
	std::vector<Move> moves;
	getMoves(pos, moves, checkConsidered);
	return moves;
}

void ChessBoard::getMoves(sf::Vector2i pos, std::vector<Move>& moves, bool checkConsidered) {
	Piece* piece = getPiece(pos);
	if (piece == nullptr) {
		return;
	}
	switch (piece->type)
	{
	case King:
		getKingMoves(pos, moves, checkConsidered);
		break;
	case Queen:
		getTowerMoves(pos, moves, checkConsidered);
		getBishopMoves(pos, moves, checkConsidered);
		break;
	case Bishop:
		getBishopMoves(pos, moves, checkConsidered);
		break;
	case Knight:
		getKnightMoves(pos, moves, checkConsidered);
		break;
	case Tower:
		getTowerMoves(pos, moves, checkConsidered);
		break;
	case Pawn:
		getPawnMoves(pos, moves, checkConsidered);
		break;
	default:
		break;
	}
}

std::vector<Move> ChessBoard::getMoveOnPos(sf::Vector2i pos) {
	std::vector<Move> moves;
	if (historyMoves.size() >= 1) {
		std::vector<Move> allMoves = historyMoves.top();
		for (Move& move : allMoves) {
			if (move.begin == pos) {
				moves.push_back(move);
			}
		}
	}
	return moves;
}

Move ChessBoard::getSpecificMove(sf::Vector2i start, sf::Vector2i end) {
	if (historyMoves.size() >= 1) {
		std::vector<Move> allMoves = historyMoves.top();
		for (Move& move : allMoves) {
			if (move.begin == start && move.end == end) {
				return move;
			}
		}
	}
	return Move();
}

bool ChessBoard::testMove(Move& move) {
	movePiece(move);
	int idPlayer = move.player;
	Piece* king = pieces[idPlayer][12];
	bool check = isThreatenedBy(king->pos, otherPlayer(idPlayer));
	unMovePiece(move);
	return !check;
}

void ChessBoard::addMove(sf::Vector2i start, sf::Vector2i end, std::vector<Move>& moves, bool checkConsidered, Tag tag) {
	Move move = Move(getPiece(start), end);
	move.tag = tag;
	if (isCorrectMove(move)) {
		if (!checkConsidered) {
			moves.push_back(move);
		}
		else if (testMove(move)) {
			moves.push_back(move);
		}
	}
}

void ChessBoard::getPawnMoves(sf::Vector2i start, std::vector<Move>& moves, bool checkConsidered) {
	Piece* piece = getPiece(start);

	// Sens d'avancee des pions
	int sign = (piece->player == 0 ? 1 : -1);

	int x = start.x;
	int y = start.y;
	sf::Vector2i end;

	Tag tag;
	if ((y == 1 && sign == -1) || (y == 6 && sign == 1)) {
		tag = Tag::Promotion;
	}
	else {
		tag = Tag::NoneTag;
	}

	if (isOnBoard(x, y + sign)) {
		Piece* frontSpot = getPiece(x, y + sign);
		if (frontSpot == nullptr) {
			// Avancée d'une case
			end.x = x;
			end.y = y + sign;
			addMove(start, end, moves, checkConsidered, tag);
			if (piece->nbMove == 0) {
				if (isOnBoard(x, y + 2 * sign)) {
					Piece* nextSpot = getPiece(x, y + 2 * sign);
					if (nextSpot == nullptr) {
						// Avancée de deux cases
						end.y = y + 2 * sign;
						addMove(start, end, moves, checkConsidered, Tag::JumpTwoCases);
					}
				}
			}
		}
	}

	// Prise
	if (isOnBoard(x - 1, y + sign)) {
		Piece* leftSpot = getPiece(x - 1, y + sign);
		if (leftSpot != nullptr && leftSpot->player != piece->player) {
			end.x = x - 1;
			end.y = y + sign;
			addMove(start, end, moves, checkConsidered, tag);
		}
	}
	if (isOnBoard(x + 1, y + sign)) {
		Piece* rightSpot = getPiece(x + 1, y + sign);
		if (rightSpot != nullptr && rightSpot->player != piece->player) {
			end.x = x + 1;
			end.y = y + sign;
			addMove(start, end, moves, checkConsidered, tag);
		}
	}

	// Prise en passant
	if (isOnBoard(x - 1, y)) {
		Piece* sideLeft = getPiece(x - 1, y);
		if (sideLeft != nullptr && sideLeft->type == Pawn && sideLeft->player != piece->player && sideLeft->hasJustMoveTwoCases) {
			end.x = x - 1;
			end.y = y + sign;
			addMove(start, end, moves, checkConsidered, Tag::EnPassant);
		}
	}
	if (isOnBoard(x + 1, y)) {
		Piece* sideRight = getPiece(x + 1, y);
		if (sideRight != nullptr && sideRight->type == Pawn && sideRight->player != piece->player && sideRight->hasJustMoveTwoCases) {
			end.x = x + 1;
			end.y = y + sign;
			addMove(start, end, moves, checkConsidered, Tag::EnPassant);
		}
	}
}

void ChessBoard::getKnightMoves(sf::Vector2i start, std::vector<Move>& moves, bool checkConsidered) {
	sf::Vector2i end;

	// On cherche un cavalier qui menace la case
	sf::Vector2i* possibilities = getKnightEmplacement(start);

	Piece* movingPiece = getPiece(start);
	for (int i = 0; i < 8; i++) {
		end = possibilities[i];
		if (isOnBoard(end)) {
			Piece* piece = getPiece(end);
			if (piece == nullptr || piece->player != movingPiece->player) {
				addMove(start, end, moves, checkConsidered);
			}
		}
	}
}

void ChessBoard::getBishopMoves(sf::Vector2i start, std::vector<Move>& moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(start);

	// Les 4 diagonales possibles
	sf::Vector2i directions[4] = { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} };

	for (auto& dir : directions) {
		getDirectionMove(movingPiece, dir, moves, checkConsidered);
	}
}

void ChessBoard::getTowerMoves(sf::Vector2i start, std::vector<Move>& moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(start);

	// Les 4 directions possibles : Droite, Gauche, Bas, Haut
	sf::Vector2i directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	for (auto& dir : directions) {
		getDirectionMove(movingPiece, dir, moves, checkConsidered);
	}
}

void ChessBoard::getDirectionMove(Piece* movingPiece, sf::Vector2i dir, std::vector<Move>& moves, bool checkConsidered) {
	sf::Vector2i start(movingPiece->pos);
	sf::Vector2i end;

	int diffX = dir.x;
	int diffY = dir.y;

	bool next = true;
	int j = 1;
	do {
		end.x = start.x + j * diffX;
		end.y = start.y + j * diffY;
		Piece* piece = getPiece(end);

		// On a atteint le bord => on arrete
		if (!isOnBoard(end)) {
			next = false;
		}

		// Espace libre => on continue
		else if (piece == nullptr) {
			addMove(start, end, moves, checkConsidered);
			next = true;
			j++;
		}
		// Piece présente => on arrête
		else {
			if (piece->player != movingPiece->player) {
				addMove(start, end, moves, checkConsidered);
			}
			next = false;
		}
	} while (next);
}

void ChessBoard::getKingMoves(sf::Vector2i start, std::vector<Move>& moves, bool checkConsidered) {
	int x = start.x;
	int y = start.y;
	sf::Vector2i end;

	Piece* king = getPiece(start);

	// Déplacement classiques
	int idPlayer = king->player;
	for (int diffX = -1; diffX < 2; diffX++) {
		for (int diffY = -1; diffY < 2; diffY++) {
			if (diffX == 0 && diffY == 0) {
				continue;
			}
			end.x = x + diffX;
			end.y = y + diffY;
			if (isOnBoard(end)) {
				Piece* piece = getPiece(end);
				if (piece == nullptr || piece->player != idPlayer) {
					addMove(start, end, moves, checkConsidered);
				}
			}
		}
	}

	// Roques
	if (king->nbMove == 0) {
		Piece* towerSmallRook = getPiece(7, y);
		sf::Vector2i knightPos = sf::Vector2i(6, y);
		sf::Vector2i bishopPos = sf::Vector2i(5, y);
		Piece* knight = getPiece(knightPos);
		Piece* bishop = getPiece(bishopPos);
		if (knight == nullptr
			&& bishop == nullptr
			&& towerSmallRook != nullptr
			&& towerSmallRook->nbMove == 0
			&& !isThreatenedBy(knightPos, otherPlayer(idPlayer))
			&& !isThreatenedBy(bishopPos, otherPlayer(idPlayer))
			&& !isThreatenedBy(king->pos, otherPlayer(idPlayer))
			) {
			end.x = x + 2;
			end.y = y;
			addMove(start, end, moves, checkConsidered, Tag::KingSideCastling);
		}

		Piece* towerBigRook = getPiece(0, y);
		sf::Vector2i knight2Pos = sf::Vector2i(1, y);
		sf::Vector2i bishop2Pos = sf::Vector2i(2, y);
		sf::Vector2i queenPos = sf::Vector2i(3, y);
		Piece* knight2 = getPiece(knight2Pos);
		Piece* bishop2 = getPiece(bishop2Pos);
		Piece* queen = getPiece(queenPos);
		if (bishop2 == nullptr
			&& knight2 == nullptr
			&& queen == nullptr
			&& towerBigRook != nullptr
			&& towerBigRook->nbMove == 0
			&& !isThreatenedBy(bishop2Pos, otherPlayer(idPlayer))
			&& !isThreatenedBy(queenPos, otherPlayer(idPlayer))
			&& !isThreatenedBy(king->pos, otherPlayer(idPlayer))
			) {
			end.x = x - 2;
			end.y = y;
			addMove(start, end, moves, checkConsidered, Tag::QueenSideCastling);
		}
	}
}

std::string ChessBoard::generateBoardId() const {
	std::string id = "";
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = getPiece(x, y);
			id += pieceToStr(piece);
		}
	}
	return id;
}

std::string ChessBoard::getMoveSymbol(Move move) {
	std::string symbol = "";

	if (move.tag == Tag::EmptyMove) {
		return "";
	}
	if (move.tag == Tag::KingSideCastling) {
		symbol += "O-o";
	}
	else if (move.tag == Tag::QueenSideCastling) {
		symbol += "O-o-o";
	}
	else {
		symbol += getId(move.piece);

		// On lève une éventuelle ambiguïté
		std::vector<Move> validMoves = historyMoves.top();
		Move sameEndMove;
		for (Move& otherMove : validMoves) {
			if (move.piece->type == otherMove.piece->type && move.end == otherMove.end) {
				if (move.begin != otherMove.begin) {
					sameEndMove = otherMove;
					break;
				}
			}
		}

		if (sameEndMove.tag != Tag::EmptyMove) {
			if (move.begin.x != sameEndMove.begin.x) {
				symbol += getSymbolPosX(move.begin.x);
			}
			else {
				symbol += getSymbolPosY(move.begin.y);
			}
		}

		if (move.destroyed != nullptr) {
			symbol += "x";
		}
		symbol += getSymbolPosition(move.end);
	}

	if (move.tag == Tag::Promotion) {
		symbol += "=D";
	}

	if (isCheckMate()) {
		symbol += "#";
	}
	else if (isCheck(move.piece->player)) {
		symbol += "+";
	}

	return symbol;
}

std::string ChessBoard::getMoveSymbol(int index) {
	std::vector<Move> validMoves = historyMoves.top();
	return getMoveSymbol(validMoves[index]);
}

void ChessBoard::printMove(Move move) {
	std::cout << getMoveSymbol(move) << " / " << tagToStr(move.tag) << std::endl;
}

void ChessBoard::printValidMoves() {
	std::cout << "Valid Moves : ";
	std::vector<Move> validMoves = historyMoves.top();
	for (Move& move : validMoves) {
		printMove(move);
	}
	std::cout << std::endl;
}

void ChessBoard::printBoard() const {
	for (int y = 0; y < 8; y++) {
		for (int x = 0; x < 8; x++) {
			Piece* piece = getPiece(x, y);
			std::cout << pieceToStr(piece);
			if (x != 7) {
				std::cout << "-";
			}
		}
		std::cout << std::endl;
	}
}

uint64_t ChessBoard::hashBoard() const {
	uint64_t hash = 0;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = getPiece(x, y);
			if (piece) {
				int squareIndex = y * 8 + x;
				int pieceType = getType(piece);
				hash ^= zobristTable[squareIndex][pieceType];
			}
		}
	}
	return hash;
}