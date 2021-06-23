#include "Game.h"


//contructor
Game::Game(int resolucion_x, int resolucion_y, string titulo)
{
	
	// tamaño de la camara
	camara_width = 320.0; 
	camara_height = 240.0; 
	
	ventana = new RenderWindow(VideoMode(resolucion_x, resolucion_y), titulo);

	//ventana = new RenderWindow(VideoMode(resolucion_x, resolucion_y), titulo, Style::Fullscreen); // para ponerlo en pantalla completa
	
	camara = new View(Vector2f(0.0,0.0), Vector2f(camara_width, camara_height)); //centro de la camara, ancho x alto
		
	//Limitamos los framerates a 60 fps
	fps = 60;
	ventana-> setFramerateLimit(fps);
	
	// fuentes de texto
	fuente1 = new Font();
	fuente1->loadFromFile("editundo.ttf");
	texto_numVuelta = new Text();
	texto_numVuelta->setFont(*fuente1);
	texto_numVuelta->setColor(Color::Yellow);
	texto_numVuelta->setPosition(550,0);
	
	texto_tiempoVuelta = new Text();
	texto_tiempoVuelta->setFont(*fuente1);
	texto_tiempoVuelta->setColor(Color::Yellow);
	texto_tiempoVuelta->setPosition(50,0);
	
	texto_countDown = new Text();
	texto_countDown->setFont(*fuente1);
	texto_countDown->setColor(Color::Red);
	
	
	//reloj para el conteo incial
	clock = new Clock();
	countDown = new Time();
	
	cargarGraficos();  // metodo para dibujar los graficos del juego
	
	Music musica; //para reproducir la musica del juego

	if (!musica.openFromFile("Push.wav"))
	{
		//error		
	}

	musica.play();
		
	initialSettings();

	mostrarMenu();	// muestra el menu incial
		
	run();
		
}



void Game::run()
{

	
	SoundBuffer buffer;
	if (!buffer.loadFromFile("Go.wav"))
	{
		// Error...
	}
	
	Sound sound;
	sound.setBuffer(buffer);
	sound.setVolume(100);
	sound.play();
		
	*countDown = clock->restart(); // inicio el reloj para el countdown

		
	while (!GameOver)
	{
				
		*countDown = clock->getElapsedTime(); 
		
		
		ventana->clear(Color::Black);
		ventana->setView(*camara);
		camara->setCenter(posicionCamara()); // posicion de la camara. Sigue al jugador
		
		ventana->draw(pista1.get_SpritePista());	//dibujo el circuito
		ventana->draw(spr_finishLine);				//dibujo la linea de llegada
		
		
		for (int i=0; i<13; i++)
		{
			ventana->draw(spr_checkpoint[i]);	 // dibujo los checkpoints
		}
		
		ventana->draw(jugador1.get_SpriteCar());	//dibujo al auto del jugador
				
		for (int i = 0; i<3; i++)
		{
			ventana->draw(car[i].get_SpriteCar());	//dibujo los autos de la CPU
		}
					
		ventana->setView(ventana->getDefaultView()); //reseteo para dibujar el HUD (numero de vuelta, tiempo y posicion)
		
	
		startCountDown(); // 3,2,1...Go! 

		if (start == true && jugador1.lap <= total_vueltas)
		{
			texto_numVuelta->setString("Lap: " + to_string(jugador1.lap)); // muestro en pantalla el numero de vuelta
			ventana->draw(*texto_numVuelta);
			ventana->draw(spr_helmet);
			
			texto_tiempoVuelta->setString("Time: " + to_string(jugador1.get_Time())); // muestro en pantalla el tiempo de vuelta
			ventana->draw(*texto_tiempoVuelta);		
		
			jugador1.eventosAuto();
		}

		jugador1.get_Time(); //empiezo a contar el tiempo de vuelta del jugador
				
		for (int j = 0; j<3; j++)
		{
			car[j].get_Time(); //empiezo a contar el tiempo de vuelta de la CPU
		}

		ventana->display();

		calcularVuelta();
		
		jugador1.moverAuto();
		
		// IA de los autos de la CPU
		for (int i = 0; i<3; i++)
		{
			car[i].moverAuto();
			car[i].nextWaypoint(); 
			car[i].set_Position(); 
		}
				
		procesarColisiones();


		//reduzo la velocidad a cero cuando se completaron todas las vueltas
		if ((jugador1.lap > total_vueltas) && (jugador1.get_Time() > 0.5))
		{
			jugador1.setVelocidad(0.0);
			
		}

		
		for (int i = 0; i<3; i++)
		{
			if ((car[i].lap > total_vueltas) && (car[i].get_Time() > 0.5))
			{
				car[i].setVelocidad(0.0);

			}
		}


		// muestra la pantalla de las posiciones finales una vez que todos los autos llegaron a la meta
		if ((car[0].lap > total_vueltas) && (car[1].lap > total_vueltas) && (car[2].lap > total_vueltas) && (jugador1.lap > total_vueltas))
		{
			if (jugador1.get_Time() > 3)
			{
				GameOver = true;
			}
		}

	}
	
	if (GameOver == true)
	{
		calcularRanking();
		mostrarPosiciones();
	}


}

void Game::cargarGraficos()
{

    //Ocultamos el cursor del sistema
    ventana->setMouseCursorVisible(false);
	
	// Title Screen
	mat_titleScreen.loadFromFile("TitleScreen.png");
	spr_titleScreen.setTexture(mat_titleScreen);


	// Title Screen
	mat_controls.loadFromFile("controlScreen.png");
	spr_controls.setTexture(mat_controls);

	// HUD
	mat_helmet.loadFromFile("HUD_helmet.png");
	spr_helmet.setTexture(mat_helmet);
	spr_helmet.setPosition(10,8);
	spr_helmet.setScale(2,2);

	// Finih Line
	mat_finishLine.loadFromFile("checkFinish.png");
	spr_finishLine.setTexture(mat_finishLine);
	spr_finishLine.setPosition(440,920); // posicion de la meta. En cada pista deberia cambiar. Ver de automatizar posicion.
	spr_finishLine.setColor(Color::Transparent); //lo hago invisible 
	

	// Posiciones finales
	mat_posiciones.loadFromFile("race_positions.png");
	spr_posiciones.setTexture(mat_posiciones);
	spr_posiciones.scale(2.33,3.33);
	
	// defino las posiciones donde dibujar los checkpoints
	int checkpoints_pos[13][2] = {{468, 600}, {754, 600}, {746, 1114}, {1347, 1244}, {1421, 914}, {1152, 600}, {1139, 212}, {949, 221}, {693, 281}, {348, 87}, {86, 320}, {79, 1184}, {462, 1176}};

	// checkpoints
	mat_checkpoint.loadFromFile("checkpoints.png");
	
	for (int i = 0; i<13 ;i++)
	{
		spr_checkpoint[i].setTexture(mat_checkpoint);
		spr_checkpoint[i].setPosition(checkpoints_pos[i][0],checkpoints_pos[i][1]);
		spr_checkpoint[i].setColor(Color::Transparent); //hago invisibles los checkpoints
	}
	
}



// Determinamos la posicion de la camara siguiendo al jugador. Evitamos mostrar bordes negros fuera de la pista de carreras.
Vector2f Game::posicionCamara()
{
		
	if ((jugador1.get_Position().x  > camara_width/2) && (jugador1.get_Position().x < pista1.getSizeX()-camara_width/2)) 
	{
		offsetX = jugador1.get_Position().x;
	}

	if (jugador1.get_Position().x < camara_width/2)	
	{
		offsetX = camara_width/2;
	}

	if (jugador1.get_Position().x > pista1.getSizeX()-camara_width/2)	
	{	
		offsetX = pista1.getSizeX()-camara_width/2;
	}


	
	if ((jugador1.get_Position().y > camara_height/2) && (jugador1.get_Position().y < pista1.getSizeY()-camara_height/2)) 
	{
		offsetY = jugador1.get_Position().y;
	}

	if (jugador1.get_Position().y < camara_height/2)	
	{
		offsetY = camara_height/2;
	}
		
	if (jugador1.get_Position().y > pista1.getSizeY()-camara_height/2)	
	{	
		offsetY = pista1.getSizeY()-camara_height/2;
	}

	Vector2f posicion(offsetX, offsetY);
	return posicion;
}



void Game::procesarEventos()
{

	while (ventana->pollEvent(evento))
	{
		switch(evento.type)
		{
			case Event::Closed: //evento para detectar que se cierra la ventana clickeando en X
				ventana->close();
				exit(1);
			break;
						
			case Event::KeyPressed: 
				if (Keyboard::isKeyPressed(Keyboard::Escape)) // para cerrar el juego presionando la tecla ESC
				{
					exit(1);
				}
				else if (Keyboard::isKeyPressed(Keyboard::Return)) // tecla ENTER
				{
					GameOver = false;
				}
					
			break;
					
			case Event::Resized: 

				float aspectRatio = float(ventana->getSize().x / ventana->getSize().y);
				camara->setSize(camara_width * aspectRatio, camara_height* aspectRatio);
				//ResizeView(*ventana, *camara);
			break;
		}
	}	
}



void Game::calcularVuelta()
{
	
	////////// TIEMPO VUELTA JUGADOR ///////////////

	// verifico que el auto del jugador este colisionando con un checkpoint
	for (int i = 0; i<13; i++)
	{
		if (jugador1.get_SpriteCar().getGlobalBounds().intersects(spr_checkpoint[i].getGlobalBounds()))
		{
			if (jugador1.checkpointsChecked[i] == 0)
			{
				jugador1.checkpointsChecked[i] = 1;
				jugador1.totalChecked++;
				jugador1.checkTime = jugador1.get_Time();
				cout<<jugador1.totalChecked<<endl;
				break;
			}
		}
	}

	
	//verifico si el jugador llego a la meta, y si pasó por todos los checkpoints. De ser así, sumo 1 vuelta y guardo el tiempo de vuelta en un array
	if (jugador1.get_SpriteCar().getGlobalBounds().intersects(spr_finishLine.getGlobalBounds()))
	{
		if (jugador1.totalChecked == 13)
		{
			jugador1.totalChecked = 0;
			jugador1.lapTime[jugador1.lap-1] = jugador1.get_Time(); // guardo el tiempo de vuelta en un array
			jugador1.totalTime +=	jugador1.lapTime[jugador1.lap-1]; //sumo el tiempo de vuelta al tiempo total
			
			//para debug muestro en consola el tiempo de vuelta y el tiempo total.
			cout<<"Jugador Tiempo Vuelta N"<<jugador1.lap<<": "<<jugador1.lapTime[jugador1.lap-1]<<endl;
			cout<<"Jugador Tiempo TOTAL "<<jugador1.totalTime<<endl;
			
			for (int i = 0; i < 13; i++)
			{
				jugador1.checkpointsChecked[i] = 0; // reseteo los checkpoints
			}
			
			jugador1.lap++;
			jugador1.restartTime(); // reseteo tiempo de vuelta
			
		}
		
	}

	////////// TIEMPO VUELTA CPU  ///////////////


	for (int j = 0; j<3; j++)
	{
		for (int i = 0; i<13; i++)
		{
			if (car[j].get_SpriteCar().getGlobalBounds().intersects(spr_checkpoint[i].getGlobalBounds()))
			{
				if (car[j].checkpointsChecked[i] == 0)
				{
					car[j].checkpointsChecked[i] = 1;
					car[j].totalChecked++; // sumo todos los checkpoints que ya fueron colisionados
					car[j].checkTime = car[j].get_Time();
					
					break;
				}
			}
	
		}
	}

	// calculo numero de vuelta y tiempo para los autos manejados por la CPU
	
	for (int j = 0; j<3; j++)
	{
		if (car[j].get_SpriteCar().getGlobalBounds().intersects(spr_finishLine.getGlobalBounds()))
		{
			
			if (car[j].totalChecked == 13)
			{
				car[j].totalChecked = 0;
				car[j].lapTime[car[j].lap-1] = car[j].get_Time();
				car[j].totalTime +=	car[j].lapTime[car[j].lap-1];
				
				cout<<"Tiempo Vuelta CPU"<<j<<" : "<<car[j].lapTime[car[j].lap-1]<<endl;
				cout<<"Tiempo TOTAL CPU"<<j<<" :"<<car[j].totalTime<<endl;
				
				for (int i = 0; i < 13; i++)
				{
					car[j].checkpointsChecked[i] = 0; // reseteo los checkpoints
				}
				
				car[j].restartTime(); // reseteo tiempo de vuelta
				car[j].lap++;
				
					
			}
		}
	}
	
}


void Game::procesarColisiones()
{
	//colisiones entre los autos controlados por la CPU
		
	for(int i = 0; i<3; i++)
	for(int j = 0; j<3; j++)
	{
		int dx = 0;
		int dy = 0;
		while ((dx*dx + dy*dy) < 4*R*R)
		{
						
			car[i].setX(dx/10.0);
			car[i].setY(dy/10.0);
			car[j].setX(-dx/10.0);
			car[j].setY(-dy/10.0);
		
			dx = car[i].getX()-car[j].getX();
			dy = car[i].getY()-car[j].getY();
								
			if (!dx && !dy) 
				break;
		}
	}
	
	//colisiones entre autos de la CPU y auto del jugador
	for (int k = 0; k<3; k++)
	{
		int dx = 0;
		int dy = 0;
		while ((dx*dx + dy*dy) < 4*R*R)
		{
			car[k].setX(dx/10.0);
			car[k].setY(dy/10.0);
			jugador1.setX(-dx/10.0);
			jugador1.setY(-dy/10.0);
			dx = car[k].getX()-jugador1.getX();
			dy = car[k].getY()-jugador1.getY();
			if (!dx && !dy) 
				break;
		}
	}

	
}
	

void Game::calcularRanking()
{

	// calculo las posiciones tomando el tiempo de vuelta:
	vector<vector<double>> val_and_id;
	val_and_id.resize(4);
	
	for (int i = 0; i < 4; i++) 
	{
		val_and_id[i].resize(2); 
	}
	//en la primer columna guardo los tiempos, en la segunda guardo los indices.

	val_and_id[0][0] = car[0].totalTime;
	val_and_id[1][0] = car[1].totalTime;
	val_and_id[2][0] = car[2].totalTime;
	val_and_id[3][0] = jugador1.totalTime;;
	
	val_and_id[0][1] = 0.0;
	val_and_id[1][1] = 1.0;
	val_and_id[2][1] = 2.0;
	val_and_id[3][1] = 3.0;


	sort(val_and_id.begin(), val_and_id.end()); // ordeno los valores de menor a mayor
	cout<<"Index \t"<<"Value \n";
	for (int i = 0; i < 4; i++) 
	{
		cout<<val_and_id[i][1]<< "\t" << val_and_id[i][0] << "\n";
	}

	primero = val_and_id[0][1];
	segundo = val_and_id[1][1];
	tercero = val_and_id[2][1];
	cuarto = val_and_id[3][1];

	switch(primero)
	{
		case 0:
			first = "green";
		break;
		case 1:
			first = "blue";
		break;
		case 2:
			first = "yellow";
		break;
		case 3:
			first = "red";
		break;
	}


	switch(segundo)
	{
		case 0:
			second = "green";
		break;
		case 1:
			second = "blue";
		break;
		case 2:
			second = "yellow";
		break;
		case 3:
			second = "red";
		break;
	}

	switch(tercero)
	{
		case 0:
			third = "green";
		break;
		case 1:
			third = "blue";
		break;
		case 2:
			third = "yellow";
		break;
		case 3:
			third = "red";
		break;
	}

	switch(cuarto)
	{
		case 0:
			fourth = "green";
		break;
		case 1:
			fourth = "blue";
		break;
		case 2:
			fourth = "yellow";
		break;
		case 3:
			fourth = "red";
		break;
	}

	// Para debug, muestro en consola las posiciones finales
	cout<<"POSICIONES FINALES"<<endl;
	cout<<"1st: "<<first<<endl;
	cout<<"2nd: "<<second<<endl;
	cout<<"3rd: "<<third<<endl;
	cout<<"4rd: "<<fourth<<endl;

}



void Game::startCountDown()
{
	// defino tamaño del texto y posicion
	texto_countDown->setCharacterSize(240);
	FloatRect textRect = texto_countDown->getLocalBounds();
	texto_countDown->setOrigin(textRect.width/2, textRect.height/2);
	texto_countDown->setPosition(Vector2f(camara_width, camara_height/2));

	// comienzo la cuenta regresiva:
	if (start == false)
	{
	
		if ((countDown->asSeconds() > 0.5 ) && (countDown->asSeconds() < 1.5))
		{
			texto_countDown->setString("3");
			ventana->draw(*texto_countDown);
		}

		if ((countDown->asSeconds() > 1.5 ) && (countDown->asSeconds() < 2.5))
		{
			texto_countDown->setString("2");
			ventana->draw(*texto_countDown);
		}
		
		if ((countDown->asSeconds() > 2.5 ) && (countDown->asSeconds() < 3.5))
		{
			texto_countDown->setString("1");
			ventana->draw(*texto_countDown);
		}

		if ((countDown->asSeconds() > 3.5 ) && (countDown->asSeconds() < 4.5))
		{
			texto_countDown->setString("GO!");
			ventana->draw(*texto_countDown);
		}

		// cuando termina la cuenta regresiva, activo la velocidad de los autos de la CPU
		if (countDown->asSeconds() >= 4.5)
		{
			car[0].setVelocidad(3.0);
			car[1].setVelocidad(3.0);
			car[2].setVelocidad(3.0);
			jugador1.setVelocidad(1.0);
			jugador1.restartTime();
			car[0].restartTime();
			car[1].restartTime();
			car[2].restartTime();
			start = true;
		}
	}


}

void Game::mostrarMenu()
{
	//dibujo el menu inicial. Mantengo el loop hasta que se presiona la tecla "ENTER" (metodo procesarEventos)
	
	ventana->clear(Color::Black);
	ventana->draw(spr_titleScreen);	
	ventana->display();
	
	while(GameOver)
	{
		procesarEventos(); 
	}

	// Dibujo la pantalla donde se muestran los controles
	ventana->clear(Color::Black);
	ventana->draw(spr_controls);	
	ventana->display();
	Sleep(3000);
	
}


void Game::mostrarPosiciones()
{
	string path1, path2, path3, path4;
	path1 = first + ".png";
	path2 = second + ".png";
	path3 = third + ".png";
	path4 = fourth + ".png";

	mat_casco1.loadFromFile(path1);
	spr_casco1.setTexture(mat_casco1);
	spr_casco1.setScale(5,5);

	mat_casco2.loadFromFile(path2);
	spr_casco2.setTexture(mat_casco2);
	spr_casco2.setScale(5,5);

	mat_casco3.loadFromFile(path3);
	spr_casco3.setTexture(mat_casco3);
	spr_casco3.setScale(5,5);

	mat_casco4.loadFromFile(path4);
	spr_casco4.setTexture(mat_casco4);
	spr_casco4.setScale(5,5);

	ventana->clear(Color::Black);
	ventana->draw(spr_posiciones);	
	
	spr_casco1.setPosition(50,100);
	ventana->draw(spr_casco1);
	
	spr_casco2.setPosition(180,160);
	ventana->draw(spr_casco2);
	
	spr_casco3.setPosition(315,210);
	ventana->draw(spr_casco3);
	
	spr_casco4.setPosition(440,260);
	ventana->draw(spr_casco4);
	ventana->display();
	
	Sleep(2000);
	initialSettings();
	mostrarMenu();
	run();
	
}




void Game::initialSettings()
{

	total_vueltas = 1; // numero de vueltas de la carrera

	R = 10; // ancho de los autos en pixeles. Valor auxiliar para detectar colisiones

	GameOver = true;
		
	start = false; // cuando termina el countDown, cambia el estado a True
	
	// creo 3 numeros random distintos que se utilizan para calcular el grip de los autos de la CPU
	do
	{
		rnd1 = rand()%6-1;
		rnd2 = rand()%6-1;
		rnd3 = rand()%6-1;
	}
	while ((rnd1 == rnd2) || (rnd2 == rnd3) || (rnd1 == rnd3));

	// posiciones iniciales de los autos rivales:
	car[0].startPosition(508.0,968.0); // 1st verde
	car[1].startPosition(548.0,987.0); // 2nd azul
	car[2].startPosition(508.0,1009.0); // 3rd amarillo

	// grip afecta el angulo en el que toman las curvas
	car[0].grip = rnd1/1000; 
	car[1].grip = rnd2/1000; 
	car[2].grip = rnd3/1000; 

	car[0].setearTextura("green"); // VERDE
	car[1].setearTextura("blue"); // AZUL
	car[2].setearTextura("yellow"); // AMARILLO


	// los autos de la CPU tienen velocidad cero hasta que termina el countDown
	car[0].setVelocidad(0.0);
	car[1].setVelocidad(0.0);
	car[2].setVelocidad(0.0);
	jugador1.setVelocidad(0.0);

	
	for (int i = 0; i<3; i++)
	{
		checkedTime[i] = 0.0;
		car[i].lap = 1;
		car[i].totalChecked = 0;
		car[i].totalTime = 0;
	}
		
	for (int i = 0; i<13; i++)
	{
		car[0].checkpointsChecked[i] = 0;
		car[1].checkpointsChecked[i] = 0;
		car[2].checkpointsChecked[i] = 0;
	}

	jugador1.lap = 1;
	jugador1.nuevaPosicion(548,1032);
	jugador1.totalChecked = 0;
	jugador1.totalTime = 0;
}