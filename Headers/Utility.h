#pragma once

#include <iostream>
#include <vector>
#include <array>
#include <string_view>

#include <SFML/System/Vector2.hpp>

constexpr int chessLevelIA = 4;
constexpr int power4LevelIA = 6;

constexpr float spaceBetweenTextMenu = 80;
constexpr float spaceBetweenTextList = 30;

constexpr std::string_view HEADER_STR = "HEADER";
constexpr std::string_view MOVES_STR = "MOVES";
constexpr std::string_view SAVES_STR = "Saves";

std::vector<std::string> splitString(const std::string& str, char delimiter);
int otherPlayer(int idPlayer);
bool isOnBoard(sf::Vector2i pos);
bool isOnBoard(int x, int y);
std::array<sf::Vector2i, 8> getKnightEmplacement(sf::Vector2i pos);
std::string floatToStringWithDecimal(float value, int decimal);
sf::Vector2i PixelToChessBoard(sf::Vector2i posPixel, sf::Vector2i start, sf::Vector2i offset, bool reverse);
sf::Vector2i CoordToPixelChessBoard(sf::Vector2i posBoard, sf::Vector2i start, sf::Vector2i offset, bool reverse);