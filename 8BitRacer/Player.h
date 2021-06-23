#include "SFML\Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <stdlib.h> 
#include <windows.h>

#include "RaceTrack.h"

using namespace sf;
using namespace std;

// Esta clase controla todas las acciones del jugador.
class Player
{

public:

	Player();
	void eventosAuto();
	void moverAuto();
		
	Sprite get_SpriteCar(); // devuelve el sprite del jugador
	Vector2f get_Position(); // para determinar la ubicacion de nuestro auto

	float get_Time(); // metodo para devolver el tiempo de vuelta
	float lapTime[3]; //array donde almaceno los tiempos de cada vuelta
	void restartTime(); //metodo que resetea el tiempo de vuelta

	int lap; // numero de vuelta. 
	int checkpointsChecked[13];
	int totalChecked;
	float totalTime; //para llevar el tiempo total de carrera (suma los tiempos de todas las vueltas)
	float checkTime;
	
	void nuevaPosicion(float xn, float yn);
	void setX(float x1), setY(float y1); // setea la posicion en X e Y del auto del juegador
	float getX(), getY(); // devuelve la posicion en X e Y del auto del juegador
	void setVelocidad(float v1);


private:
	
	//atributos del jugador:
	float velocidad;
	float angulo;
	float velocidad_max;
	float aceleracion;
	float frenos;
	float velocidad_giro;
	float x, y;
		
	//creamos un reloj para llevar control del tiempo en el que aparecen los enemigos
	Clock *reloj1;
	Time *tiempo1; 
	
	RaceTrack pista;

	//sprite y textura del auto del jugador
	Sprite spr_player;
	Texture mat_player;
		
};
