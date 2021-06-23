/* 
Roberto Nicolás Baca - Marzo 2020
Modelos y Algoritmos para Videojuegos I
TP FINAL - 8Bit Racer
MRUA

*/


// Librerías
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <stdlib.h> 

#include "Game.h"

using namespace sf;
using namespace std;


int main(int argc, char * args[])
{
	
srand(time(NULL));
Game *game;

game = new Game(640,480, "8Bit Racer - Roberto Baca");

return 0;

}



