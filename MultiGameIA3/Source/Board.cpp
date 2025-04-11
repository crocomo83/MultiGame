#include "../Headers/Board.h"

#include <string>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <direct.h>

const float startX = 8;
const float startY = 7;
const float offsetX = 60.5;
const float offsetY = 60;

Board::Board()
	: validMoves()
	, repetitiveMoves()
	, history()
	, historyBoard()
{

	selectedPawn = sf::Vector2i(-1, -1);
	mousePos = sf::Vector2f(-1, -1);

	repetitiveMoves.push(0);

	history.reserve(1000);
	historyBoard.reserve(1000);

	init();
	initSprites();
	initFont();
	initHighlights();
	initDebug();
}

void Board::init() {
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

void Board::initSprites() {

	if (boardTexture.loadFromFile("Media/board.jpg"))
	{
		boardSprite = sf::Sprite(boardTexture);
	}
	else {
		std::cerr << "Error loading board texture" << std::endl;
	}

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
}

void Board::initFont() {
	font = new sf::Font();
	if (!font->loadFromFile("arial.ttf"))
	{
		// erreur...
	}
}

void Board::initHighlights() {
	if (highlightBlue.loadFromFile("Media/blue.png")) {
		blueSprite = sf::Sprite(highlightBlue, sf::IntRect(0, 0, 60, 60));
	}

	if (highlightGreen.loadFromFile("Media/green.png")) {
		greenSprite = sf::Sprite(highlightGreen, sf::IntRect(0, 0, 60, 60));
	}

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Highlight* highlightInstance = new Highlight();
			highlightInstance->activated = false;
			highlightInstance->sprite = blueSprite;
			highlightInstance->sprite.setPosition(startX + (float)x * offsetX, startY + (float)((7 - y) * offsetY));
			highlights[x][y] = highlightInstance;
		}
	}
}

void Board::resetHighlights() const{
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Highlight* highlightInstance = highlights[x][y];
			highlightInstance->activated = false;
		}
	}
}

void Board::initDebug(){
	sf::Text text;
	text.setFont(*font);
	text.setString("");
	text.setCharacterSize(24);
	text.setOutlineColor(sf::Color::White);
	text.setPosition(sf::Vector2f(10, 510));

	debugTexts.insert({ "repetitiveMoves", text });
}

bool Board::isValidMove(Move move) {
	for (int i = 0; i < validMoves.size(); i++) {
		if (equalMoves(move, validMoves[i])) {
			return true;
		}
	}
	return false;
}

bool Board::isCorrectMove(Move move) const{
	if (!isOnBoard(move.end.x, move.end.y) || !isOnBoard(move.begin.x, move.begin.y)) {
		return false;
	}

	Piece* target = getPiece(move.end.x, move.end.y);
	if (target != nullptr && target->player == move.player) {
		return false;
	}
	return true;
}

Piece* Board::select(int x, int y, int player) {
	if (selectedPawn.x == -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = 7 - (int)((float)(y - startY) / offsetY);

		if (numX < 0 || numX > 7 || numY < 0 || numY > 7) {
			return nullptr;
		}

		Piece* piece = getPiece(numX, numY);
		if (piece == nullptr || piece->player != player) {
			return nullptr;
		}

		selectedPawn.x = numX;
		selectedPawn.y = numY;

		std::vector<Move> moves = getMoves(numX, numY, true);
		
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves[i];
			highlights[move.end.x][move.end.y]->activated = true;
		}

		Piece* test = getPiece(selectedPawn.x, selectedPawn.y);

		return piece;
	}
	return nullptr;
}

bool Board::unselect(int x, int y, Piece* piece) {
	bool played = false;
	if (selectedPawn.x != -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = 7 - (int)((float)(y - startY) / offsetY);

		Move move = Move(piece, selectedPawn, sf::Vector2i(numX, numY));

		played = play(move, true);

		if (played) {
			std::cout << (piece->player == 0 ? "white" : "black") << " play " << getMoveSymbol(move) << std::endl;
			std::cout << std::endl;
		}

		resetHighlights();

		selectedPawn = sf::Vector2i(-1, -1);
	}
	return played;
}

std::vector<Move> Board::getAllMoves(int idPlayer, bool checkConsidered) {
	std::vector<Move> moves;
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		if (!piece->taken) {
			getMoves(piece->pos.x, piece->pos.y, moves, checkConsidered);
		}
	}
	return moves;
}

bool Board::isAnyMovePossible(int idPlayer) {
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		if (!piece->taken) {
			std::vector<Move> moves = getMoves(piece->pos.x, piece->pos.y, true);
			if (!moves.empty()) {
				return true;
			}
		}
	}
	return false;
}

void Board::computeValidMoves(int idPlayer) {
	validMoves.clear();
	for (int idPlayer = 0; idPlayer < 2; idPlayer++) {
		for (int i = 0; i < 16; i++) {
			Piece* piece = pieces[idPlayer][i];
			if (piece->player == idPlayer) {
				getMoves(piece->pos.x, piece->pos.y, validMoves, true);
			}
			
		}
	}
}

void Board::printValidMoves() {
	std::cout << "Valid Moves : ";
	std::vector<Move>::iterator it;
	for (it = validMoves.begin(); it != validMoves.end(); ++it) {
		std::cout << getMoveSymbol(*it) << " / ";
	}
	std::cout << std::endl;
}

bool Board::play(Move &move, bool checkValidity) {
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

	Piece* piece = getPiece(beginX, beginY);

	if (piece == nullptr) {
		return false;
	}
	else if (piece->type == Pawn) {
		// En passant
		if (abs(endX - beginX) == 1){
			Piece* dest = getPiece(endX, endY);
			if (dest == nullptr) {
				move.destroyed = getPiece(endX, beginY);
				move.destroyed->taken = true;
				piecesOnBoard[endX][beginY] = nullptr;
				move.tag = EnPassant;
			}
		}
		// Pawn moved 2 cases
		else if (abs(endY - beginY) == 2) {
			piece->hasJustMoveTwoCases = true;
			move.tag = JumpTwoCases;
		}
		// Pawn promoted
		if (endY == 0 || endY == 7) {
			piece->type = Queen;
			move.tag = Promotion;
		}
	}
	// Castling
	else if (piece->type == King && abs(endX - beginX) >= 2) {
		move.tag = Castling;
		if (endX > beginX) {
			Move move2(getPiece(7, beginY), sf::Vector2i(7, beginY), sf::Vector2i(5, beginY));
			movePiece(move2);
		}
		else {
			Move move2(getPiece(0, beginY), sf::Vector2i(0, beginY), sf::Vector2i(3, beginY));
			movePiece(move2);
		}
	}

	// Actual move
	movePiece(move);

	if (piece->type == Pawn || move.destroyed != nullptr) {
		repetitiveMoves.push(0);
	}
	else {
		repetitiveMoves.top()++;
	}

	history.push_back(move);
	historyBoard.push_back(generateBoardId());

	return true;
}

bool Board::undo() {
	int n = history.size();
	if (n == 0) {
		std::cout << "Can't undo : no move were made" << std::endl;
		return false;
	}

	Move &move = history[n - 1];
	if (n >= 2) {
		Move &previousMove = history[n - 2];
		undo(move, previousMove);
	}
	else {
		undo(move);
	}

	history.pop_back();
	historyBoard.pop_back();

	return true;
}

void Board::undo(Move &move) {
	int beginX = move.end.x;
	int beginY = move.end.y;
	int endX = move.begin.x;
	int endY = move.begin.y;

	Piece* piece = getPiece(beginX, beginY);

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
		case Castling:
			if (endX < beginX) {
				Move move2(getPiece(5, beginY), sf::Vector2i(7, beginY), sf::Vector2i(5, beginY));
				move2.destroyed = nullptr;
				unMovePiece(move2);
			}
			else {
				Move move2(getPiece(3, beginY), sf::Vector2i(0, beginY), sf::Vector2i(3, beginY));
				move2.destroyed = nullptr;
				unMovePiece(move2);
			}
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

void Board::undo(Move &move, Move &previousMove) {
	undo(move);

	// Pawn moved 2 cases
	if (previousMove.tag == JumpTwoCases) {
		piecesOnBoard[previousMove.end.x][previousMove.end.y]->hasJustMoveTwoCases = true;
	}
}

void Board::resetEnPassant(int idPlayer) const {
	for (int i = 0; i < 16; i++) {
		Piece* piece = pieces[idPlayer][i];
		piece->hasJustMoveTwoCases = false;
	}
}

void Board::movePiece(Move &move) {
	if (move.tag != Tag::EnPassant) {
		move.destroyed = getPiece(move.end.x, move.end.y);
		if (move.destroyed != nullptr) {
			move.destroyed->taken = true;
		}
	}
	Piece* piece = getPiece(move.begin.x, move.begin.y);
	piece->pos = move.end;
	piece->nbMove++;
	piecesOnBoard[move.end.x][move.end.y] = piece;
	piecesOnBoard[move.begin.x][move.begin.y] = nullptr;
}

void Board::unMovePiece(Move &move) {
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

void Board::update(int x, int y, int idPlayer) {
	mousePos = sf::Vector2f(x, y);
	if (selectedPawn.x != -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = 7 - (int)((float)(y - startY) / offsetY);

		Move move = Move(getPiece(selectedPawn), selectedPawn, sf::Vector2i(numX, numY));

		if (isValidMove(move)) {
			int realX = startX + (float)numX * offsetX;
			int realY = startY + (float)(7 - numY) * offsetY;
			greenSprite.setPosition(realX, realY);
		}
		else {
			greenSprite.setPosition(-1, -1);
		}
	}
	else {
		greenSprite.setPosition(-1, -1);
	}
}

void Board::updateDebug() {
	debugTexts["repetitiveMoves"].setString("repetitiveMoves : " + std::to_string(repetitiveMoves.top()));
}

void Board::draw(sf::RenderWindow& target) {
	target.draw(boardSprite);

	const sf::Texture* boardTexture = boardSprite.getTexture();

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Highlight* highlightInstance = highlights[x][y];
			if (highlightInstance->activated) {
				target.draw(highlightInstance->sprite);
			}
		}
	}

	if (greenSprite.getPosition().x >= 0) {
		target.draw(greenSprite);
	}

	for (int idPlayer = 0; idPlayer < 2; idPlayer++) {
		for (int i = 0; i < 16; i++) {
			Piece* piece = pieces[idPlayer][i];
			if (piece->pos != selectedPawn && !piece->taken) {
				pieceSprites[idPlayer][piece->type].setPosition(startX + (float)piece->pos.x * offsetX, startY + (float)(7 - piece->pos.y) * offsetY);
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
	
	std::map<std::string, sf::Text>::iterator it;
	for (it = debugTexts.begin(); it != debugTexts.end(); ++it) {
		target.draw(it->second);
	}
}

bool Board::isThreatenedBy(sf::Vector2i pos, int idPlayer) const{
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
	int sign = (idPlayer == 0 ? 1 : -1);

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

bool Board::isCheck(int idPlayer) {
	std::vector<Move> moves = getAllMoves(idPlayer, false);
	for (int i = 0; i < moves.size(); i++) {
		Move move = moves[i];
		Piece* target = getPiece(move.end);
		if (target != nullptr && target->type == King && target->player != idPlayer) {
			return true;
		}
	}
	return false;
}

Board::State Board::getGameState() {
	// Debut de partie
	if (history.empty()) {
		return Board::State::Normal;
	}

	Move& lastMove = history[history.size() - 1];
	int idPlayer = lastMove.player;

	// Evite d'appeler ces fonction deux fois : pour checkmate et pat
	bool check = isCheck(idPlayer);
	bool opponentCanPlay = isAnyMovePossible(otherPlayer(idPlayer));
	
	if (check && !opponentCanPlay) {
		if (idPlayer == 0) {
			return Board::State::CheckMateWhite;
		}
		else {
			return Board::State::CheckMateBlack;
		}
	}
	else if (!check && !opponentCanPlay) {
		return Board::State::Equality;
	}
	else if (isEquality()) {
		return Board::State::Equality;
	}
}

bool Board::isCheckMate() {
	// Debut de partie => pas d'echec et mat
	if (history.empty()) {
		return false;
	}

	// Pat
	Move& lastMove = history[history.size() - 1];
	return isCheck(lastMove.player) && !isAnyMovePossible(otherPlayer(lastMove.player));
}

bool Board::isEquality() {
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

float Board::eval(int player, float weightPieces, float weightMoves, float weightRandom) {
	if (isCheckMate()) {
		if (player == 0) {
			return -1000;
		}
		else {
			return 1000;
		}	
	}
	if (isEquality()) {
		return 0;
	}
	float score = 0;
	score += weightPieces * evalPieces();
	score += weightMoves * evalMoves();
	score += weightRandom * (2 * (float)rand() / (float)RAND_MAX - 1);
	return score;
}

float Board::evalPieces() const{
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

float Board::evalMoves() {
	std::vector<Move> moves0 = getAllMoves(0, false);
	std::vector<Move> moves1 = getAllMoves(1, false);
	int size0 = moves0.size();
	int size1 = moves1.size();
	int diff = moves1.size() - moves0.size();
	return (float)diff;
}

float Board::evalCenter() {
	return 0;
}

Piece* Board::getPiece(sf::Vector2i pos) const {
	return getPiece(pos.x, pos.y);
}

Piece* Board::getPiece(int x, int y) const{
	if (x < 0 || x > 7 || y < 0 || y > 7) {
		return nullptr;
	}
	return piecesOnBoard[x][y];
}

std::vector<Move> Board::getMoves(int x, int y, bool checkConsidered) {
	std::vector<Move> moves;
	getMoves(x, y, moves, checkConsidered);
	return moves;
}

void Board::getMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* piece = getPiece(x, y);
	if (piece == nullptr) {
		return;
	}
	switch (piece->type)
	{
	case King:
		getKingMoves(x, y, moves, checkConsidered);
		break;
	case Queen:
		getTowerMoves(x, y, moves, checkConsidered);
		getBishopMoves(x, y, moves, checkConsidered);
		break;
	case Bishop:
		getBishopMoves(x, y, moves, checkConsidered);
		break;
	case Knight:
		getKnightMoves(x, y, moves, checkConsidered);
		break;
	case Tower:
		getTowerMoves(x, y, moves, checkConsidered);
		break;
	case Pawn:
		getPawnMoves(x, y, moves, checkConsidered);
		break;
	default:
		break;
	}
}

bool Board::testMove(Move &move) {
	play(move, false);
	bool check = isCheck(otherPlayer(move.player));
	undo();
	return !check;
}

void Board::addMove(int startX, int startY, int endX, int endY, int player, std::vector<Move> &moves, bool checkConsidered) {
	Move move = Move(getPiece(startX, startY), sf::Vector2i(startX, startY), sf::Vector2i(endX, endY));
	if (isCorrectMove(move)) {
		if (!checkConsidered) {
			moves.push_back(move);
		}
		else if (testMove(move)) {
			moves.push_back(move);
		}
	}
}

void Board::getPawnMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* piece = getPiece(x, y);
	int player = piece->player;

	// Sens d'avancee des pions
	int sign = (piece->player == 0 ? 1 : -1);

	if (isOnBoard(x, y + sign)) {
		Piece* frontSpot = getPiece(x, y + sign);
		if (frontSpot == nullptr) {
			// Avancée d'une case
			addMove(x, y, x, y + sign, player, moves, checkConsidered);
			if (piece->nbMove == 0) {
				if (isOnBoard(x, y + 2 * sign)) {
					Piece* nextSpot = getPiece(x, y + 2 * sign);
					if (nextSpot == nullptr) {
						// Avancée de deux cases
						addMove(x, y, x, y + 2 * sign, player, moves, checkConsidered);
					}
				}
			}
		}
	}
	
	// Prise
	if (isOnBoard(x - 1, y + sign)) {
		Piece* leftSpot = getPiece(x - 1, y + sign);
		if (leftSpot != nullptr && leftSpot->player != player) {
			addMove(x, y, x - 1, y + sign, player, moves, checkConsidered);
		}
	}
	if (isOnBoard(x + 1, y + sign)) {
		Piece* rightSpot = getPiece(x + 1, y + sign);
		if (rightSpot != nullptr && rightSpot->player != player) {
			addMove(x, y, x + 1, y + sign, player, moves, checkConsidered);
		}
	}
	
	// Prise en passant
	if (isOnBoard(x - 1, y)) {
		Piece* sideLeft = getPiece(x - 1, y);
		if (sideLeft != nullptr && sideLeft->type == Pawn && sideLeft->player != piece->player && sideLeft->hasJustMoveTwoCases) {
			addMove(x, y, x - 1, y + sign, player, moves, checkConsidered);
		}
	}
	if (isOnBoard(x + 1, y)) {
		Piece* sideRight = getPiece(x + 1, y);
		if (sideRight != nullptr && sideRight->type == Pawn && sideRight->player != piece->player && sideRight->hasJustMoveTwoCases) {
			addMove(x, y, x + 1, y + sign, player, moves, checkConsidered);
		}
	}
}

void Board::getKnightMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	// On cherche un cavalier qui menace la case
	sf::Vector2i* possibilities = getKnightEmplacement(sf::Vector2i(x, y));

	Piece* movingPiece = getPiece(x, y);
	for (int i = 0; i < 8; i++) {
		sf::Vector2i newPos = possibilities[i];
		if (isOnBoard(newPos)) {
			Piece* piece = getPiece(newPos);
			if (piece == nullptr || piece->player != movingPiece->player) {
				addMove(x, y, newPos.x, newPos.y, movingPiece->player, moves, checkConsidered);
			}
		}
	}
}

void Board::getBishopMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(x, y);

	// Les 4 diagonales possibles
	sf::Vector2i directions[4] = { {1, 1}, {-1, -1}, {-1, 1}, {1, -1} };

	for (auto& dir : directions) {
		getDirectionMove(movingPiece, dir, moves, checkConsidered);
	}
}

void Board::getTowerMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(x, y);

	// Les 4 directions possibles : Droite, Gauche, Bas, Haut
	sf::Vector2i directions[4] = { {1, 0}, {-1, 0}, {0, 1}, {0, -1} };

	for (auto& dir : directions) {
		getDirectionMove(movingPiece, dir, moves, checkConsidered);
	}
}

void Board::getDirectionMove(Piece* movingPiece, sf::Vector2i dir, std::vector<Move>& moves, bool checkConsidered) {
	int x = movingPiece->pos.x;
	int y = movingPiece->pos.y;

	int diffX = dir.x;
	int diffY = dir.y;

	bool next = true;
	int j = 1;
	do {
		int newX = x + j * diffX;
		int newY = y + j * diffY;
		Piece* piece = getPiece(newX, newY);

		// On a atteint le bord => on arrete
		if (!isOnBoard(newX, newY)) {
			next = false;
		}

		// Espace libre => on continue
		else if (piece == nullptr) {
			addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
			next = true;
			j++;
		}
		// Piece présente => on arrête
		else {
			if (piece->player != movingPiece->player) {
				addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
			}
			next = false;
		}
	} while (next);
}

void Board::getKingMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* king = getPiece(x, y);
	int idPlayer = king->player;
	for (int diffX = -1; diffX < 2; diffX++) {
		for (int diffY = -1; diffY < 2; diffY++) {
			if (diffX == 0 && diffY == 0) {
				continue;
			}
			int newX = x + diffX;
			int newY = y + diffY;
			if (isOnBoard(newX, newY)) {
				Piece* piece = getPiece(newX, newY);
				if (piece == nullptr || piece->player != idPlayer) {
					addMove(x, y, newX, newY, idPlayer, moves, checkConsidered);
				}
			}
		}
	}

	// Roque
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
			addMove(x, y, x + 2, y, idPlayer, moves, checkConsidered);
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
			addMove(x, y, x - 2, y, idPlayer, moves, checkConsidered);
		}
	}
}

std::string Board::generateBoardId() const {
	std::string id = "";
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = getPiece(x, y);
			if (piece == nullptr) {
				id += " ";
			}
			else if (piece->player == 0) {
				switch (piece->type) {
					case PieceType::King:
						id += "R";
						break;
					case PieceType::Queen:
						id += "D";
						break;
					case PieceType::Bishop:
						id += "F";
						break;
					case PieceType::Knight:
						id += "C";
						break;
					case PieceType::Tower:
						id += "T";
						break;
					case PieceType::Pawn:
						id += "P";
						break;
					
				}
			}
			else {
				switch (piece->type) {
					case PieceType::King:
						id += "r";
						break;
					case PieceType::Queen:
						id += "d";
						break;
					case PieceType::Bishop:
						id += "f";
						break;
					case PieceType::Knight:
						id += "c";
						break;
					case PieceType::Tower:
						id += "t";
						break;
					case PieceType::Pawn:
						id += "p";
						break;
				}
			}
		}
	}
	return id;
}

std::string Board::getMoveSymbol(Move move) {
	std::string symbol = getId(move.piece);
	if (move.destroyed != nullptr) {
		symbol += "x";
	}
	symbol += getSymbolPosition(move.end);

	if (isCheckMate()) {
		symbol += "#";
	}
	else if (isCheck(move.piece->player)) {
		symbol += "+";
	}

	return symbol;
}

void Board::printMove(Move move) {
	std::cout << getMoveSymbol(move) << std::endl;
}