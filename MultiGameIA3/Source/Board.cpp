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
	, historyBoard()
{

	selectedPawn = sf::Vector2i(-1, -1);
	lastMove = sf::Vector2i(-1, -1);
	mousePos = sf::Vector2f(-1, -1);

	repetitiveMoves.push(0);

	init();
	initSprites();
	initFont();
	initHighlights();
	initPositions();
	initDebug();
}

void Board::init() {
	for (int x = 0; x < 8; x++) {
		pieces[x][1] = new Piece(Pawn, 1);
		pieces[x][6] = new Piece(Pawn, 0);
	}

	pieces[0][0] = new Piece(Tower, 1);
	pieces[1][0] = new Piece(Knight, 1);
	pieces[2][0] = new Piece(Bishop, 1);
	pieces[3][0] = new Piece(Queen, 1);
	pieces[4][0] = new Piece(King, 1);
	pieces[5][0] = new Piece(Bishop, 1);
	pieces[6][0] = new Piece(Knight, 1);
	pieces[7][0] = new Piece(Tower, 1);

	pieces[0][7] = new Piece(Tower, 0);
	pieces[1][7] = new Piece(Knight, 0);
	pieces[2][7] = new Piece(Bishop, 0);
	pieces[3][7] = new Piece(Queen, 0);
	pieces[4][7] = new Piece(King, 0);
	pieces[5][7] = new Piece(Bishop, 0);
	pieces[6][7] = new Piece(Knight, 0);
	pieces[7][7] = new Piece(Tower, 0);

	for (int x = 0; x < 8; x++) {
		for (int y = 2; y < 6; y++) {
			pieces[x][y] = new Piece(None, -1);
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
		//std::cerr << "Error loading pieces" << std::endl;
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
			highlightInstance->sprite.setPosition(startX + (float)x * offsetX, startY + (float)y * offsetY);
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

void Board::initPositions() const{
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			if (piece->type != None) {
				int player = piece->player;
			}
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

bool Board::isOnBoard(int x, int y) {
	return x >= 0 && x < 8 && y >= 0 && y < 8;
}

bool Board::isValidMove(Move move) {
	for (int i = 0; i < validMoves.size(); i++) {
		if (equalMoves(move, validMoves[i])) {
			return true;
		}
	}
	return false;
}

bool Board::isCorrectMove(Move move) {
	if (!isOnBoard(move.end.x, move.end.y) || !isOnBoard(move.begin.x, move.begin.y)) {
		return false;
	}

	Piece* target = pieces[move.end.x][move.end.y];
	if (target->type != None && target->player == move.player) {
		return false;
	}
	return true;
}

Piece* Board::select(int x, int y, int player) {
	if (selectedPawn.x == -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = (int)((float)(y - startY) / offsetY);

		Piece* piece = pieces[numX][numY];
		if (piece->player != player) {
			return nullptr;
		}

		if (numX >= 0 && numX < 8 && numY >= 0 && numY < 8) {
			selectedPawn.x = numX;
			selectedPawn.y = numY;
		}

		std::vector<Move> moves = getMoves(numX, numY, true);
		for (int i = 0; i < moves.size(); i++) {
			Move move = moves[i];
			highlights[move.end.x][move.end.y]->activated = true;
		}

		return piece;
	}
	return nullptr;
}

bool Board::unselect(int x, int y, Piece* piece) {
	bool played = false;
	if (selectedPawn.x != -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = (int)((float)(y - startY) / offsetY);

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
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			if (piece->type != None && piece->player == idPlayer) {
				getMoves(x, y, moves, checkConsidered);
			}
		}
	}
	return moves;
}

bool Board::isAnyMovePossible(int idPlayer) {
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			if (piece->type != None && piece->player == idPlayer) {
				std::vector<Move> moves = getMoves(x, y, true);
				if (!moves.empty()) {
					return true;
				}
			}
		}
	}
	return false;
}

void Board::computeValidMoves(int idPlayer) {
	validMoves.clear();
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			if (piece->type != None && piece->player == idPlayer) {
				getMoves(x, y, validMoves, true);
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

	int beginX = move.begin.x;
	int beginY = move.begin.y;
	int endX = move.end.x;
	int endY = move.end.y;

	Piece* piece = pieces[beginX][beginY];

	if (piece->type == Pawn) {
		// En passant
		if (abs(endX - beginX) == 1){
			Piece* dest = pieces[endX][endY];
			if (dest != nullptr && dest->type == None) {
				move.destroyed = pieces[endX][beginY];
				pieces[endX][beginY] = new Piece();
				move.tag = EnPassant;
			}
		}
		// Pawn moved 2 cases
		else if (abs(endY - beginY) == 2) {
			piece->hasJustMoveTwoCases = true;
			move.tag = JumpTwoCases;
			lastMove.x = endX;
			lastMove.y = endY;
		}
		// Pawn promoted
		else if (endY == 0 || endY == 7) {
			promotion(beginX, beginY);
			move.tag = Promotion;
		}
	}

	// Castling
	if (piece->type == King && abs(endX - beginX) >= 2) {
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

	if (piece->type == Pawn || move.destroyed->type != None) {
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

	Piece* piece = pieces[beginX][beginY];
	Piece * pawnTaken;

	switch (move.tag) {
		case NoneTag:
			break;
		case JumpTwoCases:
			piece->hasJustMoveTwoCases = false;
			break;
		case EnPassant:
			pawnTaken = new Piece(Pawn, !move.player);
			pawnTaken->hasJustMoveTwoCases = true;
			pawnTaken->nbMoves = 1;
			delete(pieces[beginX][endY]);
			pieces[beginX][endY] = pawnTaken;
			break;
		case Promotion:
			piece->type = Pawn;
			break;
		case Castling:
			if (endX < beginX) {
				Move move2(getPiece(5, beginY), sf::Vector2i(7, beginY), sf::Vector2i(5, beginY));
				move2.destroyed = new Piece();
				unMovePiece(move2);
			}
			else {
				Move move2(getPiece(3, beginY), sf::Vector2i(0, beginY), sf::Vector2i(3, beginY));
				move2.destroyed = new Piece();
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
		pieces[previousMove.end.x][previousMove.end.y]->hasJustMoveTwoCases = true;
	}
}

void Board::movePiece(Move &move) {
	move.destroyed = pieces[move.end.x][move.end.y];
	pieces[move.end.x][move.end.y] = pieces[move.begin.x][move.begin.y];
	pieces[move.begin.x][move.begin.y] = new Piece();
	pieces[move.end.x][move.end.y]->nbMoves++;
}

void Board::unMovePiece(Move &move) {
	delete(pieces[move.begin.x][move.begin.y]);
	pieces[move.begin.x][move.begin.y] = pieces[move.end.x][move.end.y];
	pieces[move.end.x][move.end.y] = move.destroyed;
	pieces[move.begin.x][move.begin.y]->nbMoves--;
}

void Board::resetLastMove() {
	pieces[lastMove.x][lastMove.y]->hasJustMoveTwoCases = false;
	lastMove = sf::Vector2i(-1, -1);
}

void Board::promotion(int x, int y) {
	pieces[x][y]->type = Queen;
}

void Board::update(int x, int y, int idPlayer) {
	mousePos = sf::Vector2f(x, y);
	if (selectedPawn.x != -1) {
		int numX = (int)((float)(x - startX) / offsetX);
		int numY = (int)((float)(y - startY) / offsetY);

		Move move = Move(getPiece(selectedPawn), selectedPawn, sf::Vector2i(numX, numY));

		if (isValidMove(move)) {
			int realX = startX + (float)numX * offsetX;
			int realY = startY + (float)numY * offsetY;
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

	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			if (x != selectedPawn.x || y != selectedPawn.y) {
				Piece* piece = pieces[x][y];
				if (piece->type != None) {
					pieceSprites[piece->player][piece->type].setPosition(startX + (float)x * offsetX, startY + (float)y * offsetY);
					target.draw(pieceSprites[piece->player][piece->type]);
				}
			}
		}
	}

	if (selectedPawn.x != -1) {
		Piece* selectedPiece = pieces[selectedPawn.x][selectedPawn.y];
		sf::Sprite* sprite = &pieceSprites[selectedPiece->player][selectedPiece->type];
		sprite->setPosition( mousePos.x - sprite->getLocalBounds().width / 2, mousePos.y - sprite->getLocalBounds().height / 2);
		target.draw(*sprite);
	}

	std::map<std::string, sf::Text>::iterator it;
	for (it = debugTexts.begin(); it != debugTexts.end(); ++it) {
		target.draw(it->second);
	}
}

bool Board::isCheck(int idPlayer) {
	std::vector<Move> moves = getAllMoves(idPlayer, false);
	for (int i = 0; i < moves.size(); i++) {
		Move move = moves[i];
		Piece* target = pieces[move.end.x][move.end.y];
		if (target->type == King && target->player != idPlayer) {
			return true;
		}
	}
	return false;
}

bool Board::isCheckMate(int idPlayer) {
	if (history.empty()) {
		return false;
	}
	Move lastMove = history[history.size() - 1];
	if (lastMove.player == idPlayer && isCheck(idPlayer) && !isAnyMovePossible(otherPlayer(idPlayer))) {
		return true;
	}
	return false;
}

bool Board::isEquality() {
	// Debut de partie => pas d'égalité
	if (history.empty()) {
		return false;
	}

	// Pat
	Move &lastMove = history[history.size() - 1];
	if (!isAnyMovePossible(otherPlayer(lastMove.player))) {
		return true;
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
	if (isCheckMate(player)) {
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
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			float rawValue = 0;
			switch (piece->type) {
				case None:
					break;
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
	return pieces[x][y];
}

std::vector<Move> Board::getMoves(int x, int y, bool checkConsidered) {
	std::vector<Move> moves;
	Piece* piece = getPiece(x, y);
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
	return moves;
}

void Board::getMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* piece = getPiece(x, y);
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
	if (isCheck(otherPlayer(move.player))) {
		undo();
		return false;
	}
	undo();
	return true;
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
	int sign = 2 * player - 1;

	Piece* frontSpot = getPiece(x, y + sign);
	if (frontSpot != nullptr && frontSpot->type == None) {
		addMove(x, y, x, y + sign, player, moves, checkConsidered);
		if (piece->nbMoves == 0) {
			Piece* nextSpot = getPiece(x, y + 2 * sign);
			if (nextSpot != nullptr && nextSpot->type == None) {
				addMove(x, y, x, y + 2 * sign, player, moves, checkConsidered);
			}
		}
	}
	Piece* leftSpot = getPiece(x - 1, y + sign);
	if (leftSpot != nullptr && leftSpot->type != None && leftSpot->player != player) {
		addMove(x, y, x - 1, y + sign, player, moves, checkConsidered);
	}
	Piece* rightSpot = getPiece(x + 1, y + sign);
	if (rightSpot != nullptr && rightSpot->type != None && rightSpot->player != player) {
		addMove(x, y, x + 1, y + sign, player, moves, checkConsidered);
	}
	Piece* sideLeft = getPiece(x - 1, y);
	if (sideLeft != nullptr && sideLeft->type == Pawn && sideLeft->player != piece->player && sideLeft->hasJustMoveTwoCases) {
		addMove(x, y, x - 1, y + sign, player, moves, checkConsidered);
	}
	Piece* sideRight = getPiece(x + 1, y);
	if (sideRight != nullptr && sideRight->type == Pawn && sideRight->player != piece->player && sideRight->hasJustMoveTwoCases) {
		addMove(x, y, x + 1, y + sign, player, moves, checkConsidered);
	}
}

void Board::getKnightMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	int possibilities[8][2];

	possibilities[0][0] = x - 2;
	possibilities[1][0] = x - 2;
	possibilities[2][0] = x + 2;
	possibilities[3][0] = x + 2;
	possibilities[4][0] = x - 1;
	possibilities[5][0] = x - 1;
	possibilities[6][0] = x + 1;
	possibilities[7][0] = x + 1;

	possibilities[0][1] = y - 1;
	possibilities[1][1] = y + 1;
	possibilities[2][1] = y - 1;
	possibilities[3][1] = y + 1;
	possibilities[4][1] = y - 2;
	possibilities[5][1] = y + 2;
	possibilities[6][1] = y - 2;
	possibilities[7][1] = y + 2;

	Piece* movingPiece = getPiece(x, y);
	for (int i = 0; i < 8; i++) {
		int newX = possibilities[i][0];
		int newY = possibilities[i][1];
		Piece* piece = getPiece(possibilities[i][0], possibilities[i][1]);
		if (piece != nullptr && (piece->type == None || piece->player != movingPiece->player)) {
			addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
		}
	}
}

void Board::getBishopMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(x, y);
	for (int i = 0; i < 4; i++) {
		int diffX = 2 * (i % 2) - 1;
		int diffY = 2 * (i / 2) - 1;
		bool next = true;
		int j = 1;
		do {
			int newX = x + j * diffX;
			int newY = y + j * diffY;
			Piece* piece = getPiece(newX, newY);
			if (piece != nullptr && (piece->type == None || piece->player != movingPiece->player)) {
				addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
				if (piece->type == None) {
					j++;
				}
				else {
					next = false;
				}
			}
			else {
				next = false;
			}
		} while (next);
	}
}

void Board::getTowerMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(x, y);
	for (int i = 0; i < 4; i++) {
		int mod = 2 * (i % 2) - 1;
		int diffX, diffY;
		if (i < 2) {
			diffX = mod;
			diffY = 0;
		}
		else {
			diffX = 0;
			diffY = mod;
		}
		bool next = true;
		int j = 1;
		do {
			int newX = x + j * diffX;
			int newY = y + j * diffY;
			Piece* piece = getPiece(newX, newY);
			if (piece != nullptr && (piece->type == None || piece->player != movingPiece->player)) {
				addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
				if (piece->type == None) {
					j++;
				}
				else {
					next = false;
				}
			}
			else {
				next = false;
			}
		} while (next);
	}
}

void Board::getKingMoves(int x, int y, std::vector<Move> &moves, bool checkConsidered) {
	Piece* movingPiece = getPiece(x, y);
	for (int diffX = -1; diffX < 2; diffX++) {
		for (int diffY = -1; diffY < 2; diffY++) {
			if (diffX == 0 && diffY == 0) {
				continue;
			}
			int newX = x + diffX;
			int newY = y + diffY;
			Piece* piece = getPiece(newX, newY);
			if (piece != nullptr && (piece->type == None || piece->player != movingPiece->player)) {
				addMove(x, y, newX, newY, movingPiece->player, moves, checkConsidered);
			}
		}
	}
	if (movingPiece->nbMoves == 0) {
		for (int side = -1; side < 2; side += 2) {
			bool next = true;
			int i = 1;
			while (next) {
				int newX = x + i * side;
				Piece* piece = getPiece(newX, y);
				if (piece != nullptr && piece->type == Tower && piece->player == movingPiece->player && piece->nbMoves == 0) {
					addMove(x, y, x + 2 * side, y, movingPiece->player, moves, checkConsidered);
					next = false;
				}
				else if (piece != nullptr && piece->type == None) {
					i++;
				}
				else {
					next = false;
				}
			}
		}
	}
}

int Board::otherPlayer(int idPlayer) {
	return (idPlayer + 1) % 2;
}

std::string Board::generateBoardId() const {
	std::string id = "";
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {
			Piece* piece = pieces[x][y];
			if (piece->type == PieceType::None) {
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
	if (move.destroyed != nullptr && move.destroyed->type != PieceType::None) {
		symbol += "x";
	}
	symbol += getSymbolPosition(move.end);

	if (isCheckMate(move.piece->player)) {
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