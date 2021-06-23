#include "SFML\Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <stdlib.h> 
#include <windows.h>


using namespace sf;
using namespace std;

//Con esta clase manejo las distintas pistas disponibles. Por ahora solo 1 pista de carreras.

class RaceTrack
{

public:

	RaceTrack();
	float getSizeX();
	float getSizeY();
	Sprite get_SpritePista();
	
private:
	
	Sprite spr_pista;
	Texture mat_pista;

};
