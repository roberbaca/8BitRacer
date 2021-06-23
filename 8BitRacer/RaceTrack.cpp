#include "RaceTrack.h"

RaceTrack::RaceTrack()
{
	mat_pista.loadFromFile("RaceTrack.png");
	mat_pista.setSmooth(true);
	spr_pista.setTexture(mat_pista);
}


// devuelve el ancho (width) de la textura que usa la pista de carreras
float RaceTrack::getSizeX() 
{
	return mat_pista.getSize().x;
}

// devuelve el alto (height) de la textura que usa la pista de carreras
float RaceTrack::getSizeY()
{
	return mat_pista.getSize().y;
}


// devuelve el sprite
Sprite RaceTrack::get_SpritePista()
{
	return spr_pista;
 }

