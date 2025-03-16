#pragma once

#include <iostream>
#include <SFML/System/Vector2.hpp>

int otherPlayer(int idPlayer);
bool isOnBoard(sf::Vector2i pos);
bool isOnBoard(int x, int y);
sf::Vector2i* getKnightEmplacement(sf::Vector2i pos);
std::string floatToStringWithDecimal(float value, int decimal);