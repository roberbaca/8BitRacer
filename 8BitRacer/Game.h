#include "SFML\Graphics.hpp"
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <stdlib.h> 
#include <windows.h>

#include "Player.h"
#include "CarAI.h"

using namespace sf;
using namespace std;

class Game
{

public:
	
	Game(int resolucion_x, int resolucion_y, string titulo); // constructor, toma como parametros la resolucion (x y) y el titulo de la ventana
	void cargarGraficos(); // metodo para dibujar la ventana y los sprites
	void procesarEventos(); // metodo para procesar los eventos (mouse y teclado)
	void run(); // metodo para procesar todo lo que transcurre mientras el juego esta abierto
	void mostrarMenu();// menu inicial del juego
	void mostrarPosiciones(); // muestra las posiciones finales de la carrera
	void calcularVuelta(); // calcula numero de vuelta y tiempo
	void calcularRanking(); // calcula posiciones de la carrera
	Vector2f posicionCamara();
	void procesarColisiones(); // metodo para procesar las colisiones entre los autos de la CPU y el jugador.
	void startCountDown(); // 3,2,1...Go !	
	void initialSettings(); // para resetear las variables a sus estados inciales


private:
	
	Event evento;
	Player jugador1; //crea un objeto jugador1
	RaceTrack pista1;
	CarAI car[3]; // creo un array con los 3 autos controlados por la CPU
	

	int primero;
	int segundo;
	int tercero;
	int cuarto;

	string first;
	string second;
	string third;
	string fourth;


	int posicion;
	int num_vuelta;
	int total_vueltas;
	float tiempo_vuelta;
	float tiempo_total;
	int checkpoints_pos[13][2];
	float checkedTime[4];

	float offsetX;
	float offsetY;

	
	float camara_width; //ancho de la camara
	float camara_height; //alto de la camara

	float rnd1, rnd2, rnd3;

	float R;


	Clock *clock;
	Time *countDown; 
		
	//cursor del mouse 
	Sprite spr_cursor;
	Texture mat_cursor;

	//meta
	Sprite spr_finishLine;
	Texture mat_finishLine;

	//checkpoints
	Sprite spr_checkpoint[13];
	Texture mat_checkpoint;

	//Pantalla de Inicio
	Sprite spr_titleScreen;
	Texture mat_titleScreen;

	//Pantalla con los controles (keyboard)
	Sprite spr_controls;
	Texture mat_controls;

	//Pantalla con las posiciones finales
	Sprite spr_posiciones;
	Texture mat_posiciones;
	
	Sprite spr_casco1;
	Texture mat_casco1;
		
	Sprite spr_casco2;
	Texture mat_casco2;

	Sprite spr_casco3;
	Texture mat_casco3;

	Sprite spr_casco4;
	Texture mat_casco4;
	
	//HUD
	Sprite spr_helmet;
	Texture mat_helmet;
	
	//creamos las fuentes para mostrar texto en pantalla
	Font *fuente1;

	Text * texto_numVuelta;
	Text * texto_tiempoVuelta;
	Text * texto_countDown;

	int fps;

	// condicion de fin del juego
	bool GameOver;
	
	// para el conteo antes de la largada
	bool start; 

	// crea la ventana
	RenderWindow *ventana; 

	View *camara;

};