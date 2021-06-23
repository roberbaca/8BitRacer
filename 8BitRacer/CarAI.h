#include "SFML\Graphics.hpp"
#include <SFML/Window.hpp>
#include <iostream>
#include <stdlib.h> 
#include <windows.h>


using namespace sf;
using namespace std;

class CarAI
{

	public:

	CarAI();
	//void eventosAuto();
	void nextWaypoint();	
	void moverAuto();
	void set_Position();
	void startPosition(float xi, float yi); 
	
	Sprite get_SpriteCar();
	Vector2f get_Position(); // para determinar la ubicacion de nuestro auto

	float get_Time(); // metodo para devolver el tiempo de vuelta
	float lapTime[3]; //array donde almaceno los tiempos de cada vuelta
	void restartTime(); //metodo que resetea el tiempo de vuelta

	int lap; // numero de vuelta. 
	int checkpointsChecked[13];
	int totalChecked;
	float totalTime; //para llevar el tiempo total de carrera (suma los tiempos de todas las vueltas)
	float checkTime; // tiempo parcial que se toma al pasar por un checkpoint
	
	float grip;
	void setearTextura(string carColor);
	void setX(float x1), setY(float y1); // setea la posicion en X e Y del auto CPU
	float getX(), getY(); // devuelve la posicion en X e Y del auto CPU
	void setVelocidad(float v1);

	
private:
	
	float velocidad;
	float angulo, beta;
	float velocidad_giro;
	float wx, wy;
	int waypoints[21][2];
	int n;
	float x,y;
	
	//creamos un reloj para llevar control del tiempo en el que aparecen los enemigos
	Clock *reloj1;
	Time *tiempo1; 
	
	//autos manejados por el CPU
	Sprite spr_car;
	Texture mat_car;
	
	
};





