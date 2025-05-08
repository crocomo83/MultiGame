#include "../Headers/Power4Board.h"

#include <SFML/Window/Event.hpp>
#include <random>

const float startX = 60;
const float startY = 60;
const float offsetX = 70;
const float offsetY = 70;
const float radius = 30;

Power4Board::Power4Board()
	: BasicBoard()
	, selectedColumn(-1)
{
	gameOver = false;
	initBoard();
	initializeZobristTable();

	historyState.push_back(State::Playing);
}

void Power4Board::update(sf::Vector2i mousePosition) {
	mousePos = mousePosition;
	selectColumn(mousePos.x);
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			int value = pieceOnBoard[x][y];
			switch (value) {
			case -1:
				colorCircles[x][y].setFillColor(sf::Color::White);
				break;
			case 0:
				colorCircles[x][y].setFillColor(sf::Color::Yellow);
				break;
			case 1:
				colorCircles[x][y].setFillColor(sf::Color::Red);
				break;
			}
		}
	}

	if (selectedColumn != -1) {
		for (int y = 5; y >= 0; y--) {
			int value = pieceOnBoard[selectedColumn][y];
			if (value == -1) {
				colorCircles[selectedColumn][y].setFillColor(sf::Color::Green);
				break;
			}
		}
	}
}

void Power4Board::render(sf::RenderWindow& window) {
	window.draw(rectangle);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			window.draw(colorCircles[x][y]);
		}
	}
}

int Power4Board::handleEvent(const sf::Event& event)
{
	int columnPlayed = -1;
	switch (event.type) {
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == sf::Mouse::Left) {
				columnPlayed = selectedColumn;
			}
			break;
		}
	if (columnPlayed == -1) {
		return columnPlayed;
	}
	std::vector<int> validMoves = getAllMoves();
	for (int i = 0; i < validMoves.size(); i++) {
		if (validMoves[i] == columnPlayed) {
			return i;
		}
	}
	std::cerr << "should not happen" << std::endl;
	return -1;
}

void Power4Board::reset()
{
}

void Power4Board::initBoard() {
	rectangle.setSize(sf::Vector2f(500, 440));
	rectangle.setFillColor(sf::Color::Blue);
	rectangle.setPosition(50, 50);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			sf::CircleShape circle(radius);
			circle.setFillColor(sf::Color::White);
			circle.setPosition(startX + x * offsetX, startY + y * offsetY);
			colorCircles[x][y] = circle;
			pieceOnBoard[x][y] = -1;
		}
	}
}

void Power4Board::initializeZobristTable() {
	std::mt19937_64 rng(42);
	std::uniform_int_distribution<uint64_t> dist;

	for (int square = 0; square < 42; ++square) {
		for (int value = 0; value < 3; ++value) {
			zobristTable[square][value] = dist(rng);
		}
	}
}

std::vector<int> Power4Board::getAllMoves() const{
	std::vector<int> moves;
	for (int x = 0; x < 7; x++) {
		if (!isColumnFull(x)) {
			moves.push_back(x);
		}
	}
	return moves;
}

size_t Power4Board::getNumberMoves() {
	return getAllMoves().size();
}

bool Power4Board::play(int index) {
	std::vector<int> validMoves = getAllMoves();
	if (index < 0 || index >= validMoves.size()) {
		std::cout << "can't play move " << index << std::endl;
		return false;
	}
	return playInColum(validMoves[index]);
}

bool Power4Board::play(std::string move)
{
	return play(std::stoi(move));
}

bool Power4Board::isColumnFull(int column) const{
	return pieceOnBoard[column][0] != -1;
}

bool Power4Board::playInColum(int index) {
	if (index < 0 || index > 7 || isColumnFull(index)) {
		std::cout << "can't play in " << index << std::endl;
		return false;
	}

	for (int y = 5; y >= 0; y--) {
		int value = pieceOnBoard[index][y];
		if (value == -1) {
			pieceOnBoard[index][y] = idCurrentPlayer;
			break;
		}
	}
	history.push_back(index);
	computeGameState();
	idCurrentPlayer = otherPlayer(idCurrentPlayer);
	return true;
}

bool Power4Board::undo() {
	if (history.size() == 0) {
		std::cout << "can't undo, no moves were made" << std::endl;
		return false;
	}
	int lastColumnPlayed = history[history.size() - 1];
	for (int y = 0; y < 6; y++) {
		int value = pieceOnBoard[lastColumnPlayed][y];
		if (value != -1) {
			pieceOnBoard[lastColumnPlayed][y] = -1;
			break;
		}
	}
	history.pop_back();
	historyState.pop_back();
	idCurrentPlayer = otherPlayer(idCurrentPlayer);
	return true;
}

bool Power4Board::isGameOver() {
	State currentState = getGameState();
	switch (currentState) {
	case Player1Win:
	case Player2Win:
	case Equality:
		return true;
	default:
		return false;
	}
}

std::string Power4Board::getPlayerName(int player) const {
	if (player == 0) {
		return "Yellow";
	}
	else {
		return "Red";
	}
}

void Power4Board::selectColumn(int mousePosX) {
	int x = mousePosX;
	if (x >= startX) {
		int column = (x - startX) / offsetX;
		if (column >= 0 && column <= 6) {
			selectedColumn = column;
		}
		else {
			selectedColumn = -1;
		}
	}
	else {
		selectedColumn = -1;
	}
}

std::vector<std::vector<int>> Power4Board::getAlignmentsLastMove() const{
	int lastColumnPlayed = history[history.size() - 1];
	int lastRowPlayed = -1;

	std::vector<std::vector<int>> lines;

	std::vector<int> line;
	for (int y = 0; y < 6; y++) {
		int value = pieceOnBoard[lastColumnPlayed][y];
		line.push_back(value);
		if (value != -1) {
			if (lastRowPlayed == -1) {
				lastRowPlayed = y;
			}
		}
	}
	lines.push_back(line);

	std::vector<int> column;
	for (int x = 0; x < 7; x++) {
		column.push_back(pieceOnBoard[x][lastRowPlayed]);
	}
	lines.push_back(column);

	int cste = lastColumnPlayed + lastRowPlayed;
	std::vector<int> diag;
	for (int x = 0; x < 7; x++) {
		if ((cste - x) >= 0 && (cste - x) <= 5) {
			diag.push_back(pieceOnBoard[x][cste - x]);
		}
	}
	lines.push_back(diag);

	cste = lastRowPlayed - lastColumnPlayed;
	std::vector<int> diag2;
	for (int x = 0; x < 7; x++) {
		if ((cste + x) >= 0 && (cste + x) <= 5) {
			diag2.push_back(pieceOnBoard[x][cste + x]);
		}
	}
	lines.push_back(diag2);

	return lines;
}

std::vector<std::vector<int>> Power4Board::getAlignments() const {
	std::vector<std::vector<int>> allLines;

	for (int x = 0; x < 7; x++) {
		std::vector<int> line;
		for (int y = 0; y < 6; y++) {
			line.push_back(pieceOnBoard[x][y]);
		}
		allLines.push_back(line);
	}

	for (int y = 0; y < 6; y++) {
		std::vector<int> column;
		for (int x = 0; x < 7; x++) {
			column.push_back(pieceOnBoard[x][y]);
		}
		allLines.push_back(column);
	}

	for (int cste = 3; cste < 8; cste++) {
		std::vector<int> diag;
		for (int x = 0; x < 7; x++) {
			if ((cste - x) >= 0 && (cste - x) <= 5) {
				diag.push_back(pieceOnBoard[x][cste - x]);
			}
		}
		allLines.push_back(diag);
	}

	for (int cste = -3; cste < 2; cste++) {
		std::vector<int> diag;
		for (int x = 0; x < 7; x++) {
			if ((cste + x) >= 0 && (cste + x) <= 5) {
				diag.push_back(pieceOnBoard[x][cste + x]);
			}
		}
		allLines.push_back(diag);
	}

	return allLines;
}

bool Power4Board::isEquality() const{
	bool isEquality = true;
	for (int x = 0; x < 7; x++) {
		if (!isColumnFull(x)) {
			isEquality = false;
			break;
		}
	}
	return isEquality;
}

BasicBoard::State Power4Board::getGameState() const{
	if (historyState.size() == 0) {
		return State::Playing;
	}
	return historyState[historyState.size() - 1];
}

void Power4Board::computeGameState()
{
	// Debut de partie
	if (history.size() == 0) {
		historyState.push_back(State::Playing);
		return;
	}

	std::vector<std::vector<int>> lines = getAlignmentsLastMove();

	for (int i = 0; i < lines.size(); i++) {
		std::vector<int> line = lines[i];
		int count = 0;
		int idPlayer = -1;
		for (int j = 0; j < line.size(); j++) {
			int value = line[j];
			if (value != -1) {
				if (value == idPlayer) {
					count++;
					if (count == 4) {
						historyState.push_back((idPlayer == 0 ? Power4Board::State::Player1Win : Power4Board::State::Player2Win));
						return;
					}
				}
				else {
					count = 1;
					idPlayer = value;
				}
			}
			else {
				count = 0;
				idPlayer = value;
			}
		}
	}

	historyState.push_back((isEquality() ? Power4Board::State::Equality : Power4Board::State::Playing));
}

void Power4Board::printValidMoves() {
}

std::pair<bool, float> Power4Board::getEvaluationEndGame(int level) {
	State state = getGameState();
	switch (state) {
	case(Playing):
		return { false, 0 };
	case(Player1Win):
		return { true, -1000 - level };
	case(Player2Win):
		return { true, 1000 + level };
	case(Equality):
		return { true, 0 };
	}
	return { false, 0 };
}

float Power4Board::getEvaluation() {
	uint64_t hash = hashBoard();
	auto it = zobristCache.find(hash);
	if (it != zobristCache.end()) {
		return it->second;
	}

	float value = eval();
	zobristCache[hash] = value;
	return value;
}

std::pair<int, int> Power4Board::getSimpleCount(std::vector<std::vector<int>> allLines) const {
	std::pair<int, int> countPawnPlayer(0, 0);

	for (int i = 0; i < allLines.size(); i++) {
		std::vector<int> line = allLines[i];
		for (int j = 0; j < line.size(); j++) {
			int value = line[j];
			if (value == 0) {
				countPawnPlayer.first++;
			}
			else if (value == 1) {
				countPawnPlayer.second++;
			}
		}
	}
	return countPawnPlayer;
}

std::pair<int, int> Power4Board::getLinesCount(std::vector<std::vector<int>> allLines, int count) const {
	std::pair<int, int> countPawnPlayer(0, 0);

	for (int i = 0; i < allLines.size(); i++) {
		std::vector<int> line = allLines[i];
		for (int j = 0; j < line.size() - 4; j++) {
			int count0 = 0;
			int count1 = 0;
			for (int k = j; k < j + 4 + 1; k++) {
				int value = line[k];
				if (value == 0) {
					count0++;
				}
				else if (value == 1){
					count1++;
				}
			}
			if (count0 == count && count1 == 0) {
				countPawnPlayer.first++;
			}
			else if (count1 == count && count0 == 0) {
				countPawnPlayer.second++;
			}
		}
	}
	return countPawnPlayer;
}

float Power4Board::eval() const {
	float score = 0;
	std::vector<std::vector<int>> allLines = getAlignments();
	std::pair<int, int> threePawnPlayer = getLinesCount(allLines, 3);
	std::pair<int, int> twoPawnPlayer = getLinesCount(allLines, 2);
	std::pair<int, int> countPlayer = getSimpleCount(allLines);

	score -= 20 * threePawnPlayer.first;
	score += 20 * threePawnPlayer.second;
	score -= 5 * twoPawnPlayer.first;
	score += 5 * twoPawnPlayer.second;
	score -= countPlayer.first;
	score += countPlayer.second;

	return score;
}

std::string Power4Board::getMoveSymbol(int index) {
	return std::to_string(index);
}

std::string Power4Board::getStringToWright(int indexMove) const
{
	std::vector<int> validMoves = getAllMoves();
	int column = validMoves[indexMove];
	return std::to_string(column);
}

std::string Power4Board::getHeader() const
{
	return "Power4\n";
}

uint64_t Power4Board::hashBoard() const {
	uint64_t hash = 0;
	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			int value = pieceOnBoard[x][y];
			int squareIndex = y * 7 + x;
			hash ^= zobristTable[squareIndex][value + 1];
		}
	}
	return hash;
}

void Power4Board::printBoard() const
{
}
