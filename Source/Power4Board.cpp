#include "../Headers/Power4Board.h"

#include <SFML/Window/Event.hpp>
#include <random>

const float startX = 60;
const float startY = 60;
const float offsetX = 70;
const float offsetY = 70;
const float radius = 30;

Power4Board::Power4Board()
	: selectedColumn(-1)
{
	gameOver = false;
	initBoard();
	initializeZobristTable();
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

int Power4Board::getNumberMoves() {
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
	idCurrentPlayer = otherPlayer(idCurrentPlayer);
	return true;
}

void Power4Board::update(sf::Vector2i pos, int idPlayer) {
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

bool Power4Board::isGameOver(std::string& messageGameOver)
{
	Power4Board::State gameState = getGameState();
	switch (gameState) {
	case(Normal):
		return false;
	case(YellowWin):
		messageGameOver = "Yellow win";
		return true;
	case(RedWin):
		messageGameOver = "Red win";
		return true;
	case(Equality):
		messageGameOver = "Equality";
		return true;
	}

	return false;
}

int Power4Board::handleEvent(sf::Vector2i mousePos, sf::Event& event)
{
	int indexDecision = -1;
	selectColumn(mousePos.x);
	switch (event.type) {
	case sf::Event::MouseButtonReleased:
		if (event.mouseButton.button == sf::Mouse::Left) {
			indexDecision = selectedColumn;
		}
		break;
	case sf::Event::KeyPressed:
		if (event.key.code == sf::Keyboard::U) {
			undo();
		}
		break;
	}
	return indexDecision;
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

void Power4Board::draw(sf::RenderWindow& target) {
	target.draw(rectangle);

	for (int x = 0; x < 7; x++) {
		for (int y = 0; y < 6; y++) {
			target.draw(colorCircles[x][y]);
		}
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

Power4Board::State Power4Board::getGameState() const {
	if (history.size() == 0) {
		return Power4Board::State::Normal;
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
						return (idPlayer == 0 ? Power4Board::State::YellowWin : Power4Board::State::RedWin);
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

	return isEquality() ? Power4Board::State::Equality : Power4Board::State::Normal;
}

void Power4Board::computeValidMoves(int idPlayer) {
}

void Power4Board::printValidMoves() {
}

std::pair<bool, float> Power4Board::getEvaluationEndGame(int level) {
	Power4Board::State gameState = getGameState();
	switch (gameState) {
	case(Normal):
		return { false, 0 };
	case(YellowWin):
		return { true, -1000 - level };
	case(RedWin):
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
	std::cout << "value : " << column << std::endl;
	return std::to_string(column);
}

std::string Power4Board::getHeader() const
{
	std::string res;
	res += "Power4\n";
	return res;
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
