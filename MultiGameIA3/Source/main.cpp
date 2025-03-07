#include <SFML/Graphics.hpp>
#include <iostream>
#include <windows.h> 
#include <stdio.h>
#include <tchar.h>

#define BUFSIZE MAX_PATH

#include "../Headers/Game.h"
#include "../Headers/Menu.h"

int main()
{
	Menu* menu = new Menu();
	menu->run_menu();
	delete menu;
	menu = nullptr;
	return EXIT_SUCCESS;
}