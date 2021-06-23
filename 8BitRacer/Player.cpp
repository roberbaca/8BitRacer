#include "Player.h"


Player::Player()
{
	velocidad = 1.0;
	angulo = 0.0;
	velocidad_max = 3.2;
	aceleracion = 0.2;
	frenos = 0.3;
	velocidad_giro = 0.08;

	checkTime = 0.0;
	
	//posiciones inciales del jugador
	x = 548.0; 
	y = 1032.0;
	
	lap = 1;

	//variables para controlar los checkpoints
	for (int i = 0; i < 13; i++)
	{
		checkpointsChecked[i] = 0; 
	}
	
	totalChecked = 0;


	mat_player.loadFromFile("car.png");
    mat_player.setSmooth(true);
    spr_player.setTexture(mat_player);
	spr_player.setScale(0.8,0.8);
	spr_player.setOrigin(10,12);
    spr_player.setPosition(300,400);
	

	// variables para controlar el tiempo
	reloj1 = new Clock();
	tiempo1 = new Time();

	totalTime = 0.0;

	for (int i = 0; i < 3; i++)
	{
		lapTime[i] = 0.0; 
	}
	
	
}


void Player::eventosAuto()
{
    //Eventos con el teclado
    bool Up = false;
    bool Right = false;
    bool Down = false;
    bool Left = false;


    if (Keyboard::isKeyPressed(Keyboard::Up)) 
    {	
		Up = true;
	}
    if (Keyboard::isKeyPressed(Keyboard::Right))
    {	
		Right = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Down)) 
    {	
		Down = true;
    }

    if (Keyboard::isKeyPressed(Keyboard::Left)) 
    {	
		Left = true;
    }


  
	if (Up && (velocidad < velocidad_max) )
    {
		if(velocidad < 0)
		{
			velocidad += frenos;
		}
		else
		{
			velocidad += aceleracion;
		}
    }	
   

    if (Down && velocidad > -velocidad_max)
    {
		if (velocidad > 0) 
		{
			velocidad -= frenos;
		}
       
        else  
		{
			velocidad -= aceleracion;
		}
    }


    if (!Up && !Down)
    {       
 		if ((velocidad - frenos) > 0) 
		{
			velocidad -= frenos;
		}
        else if ((velocidad + frenos) < 0) 
		{
			velocidad += frenos;
		}        
	    
		else 
		{	
			velocidad = 0;
		}
	}

    //angulo de giro
    if (Right && velocidad != 0)
    {
    	//angulo += velocidad_giro * velocidad/velocidad_max;
		angulo += velocidad * 0.025f;
	}


    if (Left && velocidad != 0)   
    {
    	angulo -= velocidad_giro * velocidad/velocidad_max;
    }
		
}


void Player::moverAuto()
{
	
	// posicion del auto. Evito que salga de los limites (ancho y alto) de la textura que usa la pista de carreras.

	if ((x > 0) && (x < pista.getSizeX()))
	{
		x += sin(angulo) * velocidad;
	}
	else if (x <= 0)
	{
		x = mat_player.getSize().x;
	}
	else if (x >= pista.getSizeX())
	{
		x = pista.getSizeX() - mat_player.getSize().x;
	}

	
	if ((y > 0) && (y < pista.getSizeY()))
	{
		y -= cos(angulo) * velocidad;
	}
	else if (y <= 0)
	{
		y = mat_player.getSize().y;;
	}
	else if (y >= pista.getSizeY())
	{
		y = pista.getSizeY() - mat_player.getSize().y;
	}

	// traslacion de nuestro auto
	spr_player.setPosition(x, y);

	//transformo radianes a grados sexagesimales:
	spr_player.setRotation(angulo*180/3.141593);
}


Sprite Player::get_SpriteCar()
{
	return spr_player;
}


/* posicion del jugador para determinar donde ubicar la camara. La idea es que la camara siga al jugador
   pero que no muestre los bordes negros fuera de las dimensiones de la textura de la pista de carreras.
*/

Vector2f Player::get_Position() 
{
	return spr_player.getPosition();
}


float Player::get_Time()
{
	*tiempo1 = reloj1->getElapsedTime();
	return tiempo1->asSeconds();
}

void Player::restartTime()
{
	reloj1->restart();
}


void Player::nuevaPosicion(float xn, float yn)
{
	x = xn;
	y = yn;
}


// setea la posicion en X
void Player::setX(float x1)
{
	x += x1;
}


// setea la posicion en Y
void Player::setY(float y1)
{
	y += y1;
}

// devuelve la posicion en X
float Player::getX()
{
	return x;
}

// devuelve la posicion en Y
float Player::getY()
{
	return y;
}

void Player::setVelocidad(float v1)
{
	velocidad = v1;
}
