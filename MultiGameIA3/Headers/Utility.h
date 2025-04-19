#pragma once

#include <iostream>
#include <SFML/System/Vector2.hpp>

int otherPlayer(int idPlayer);
bool isOnBoard(sf::Vector2i pos);
bool isOnBoard(int x, int y);
sf::Vector2i* getKnightEmplacement(sf::Vector2i pos);
std::string floatToStringWithDecimal(float value, int decimal);
sf::Vector2i PixelToChessBoard(sf::Vector2i posPixel, sf::Vector2i start, sf::Vector2i offset, bool reverse);
sf::Vector2i CoordToPixelChessBoard(sf::Vector2i posBoard, sf::Vector2i start, sf::Vector2i offset, bool reverse);