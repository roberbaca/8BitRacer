#include "CarAI.h"


CarAI::CarAI()
{
	velocidad = 3.0;
	angulo = 0.0;
	velocidad_giro = 0.009;
	n = 0;
	lap = 1;




	//variables para controlar los checkpoints
	for (int i = 0; i < 13; i++)
	{
		checkpointsChecked[i] = 0; 
	}
	
	totalChecked = 0;
	checkTime = 0.0;
	
	mat_car.loadFromFile("car_yellow.png");
	spr_car.setTexture(mat_car);
	spr_car.setOrigin(10,12);
	spr_car.setScale(0.8,0.8);
    
	// variables para controlar el tiempo
	reloj1 = new Clock();
	tiempo1 = new Time();
	
	totalTime = 0.0;

	for (int i = 0; i < 3; i++)
	{
		lapTime[i] = 0.0; 
	}
		
}


void CarAI::setearTextura(string colorCar)
{
	string path;
	path = "car_" + colorCar + ".png";
	mat_car.loadFromFile(path);
}


Sprite CarAI::get_SpriteCar() 
{
	return spr_car;
}



float CarAI::get_Time()
{
	*tiempo1 = reloj1->getElapsedTime();
	return tiempo1->asSeconds();
}

void CarAI::restartTime()
{
	reloj1->restart();
}



void CarAI::nextWaypoint()
{

	int waypoints[21][2] = {{525, 620}, {673, 547}, {812, 606}, 
							{817, 1137}, {945, 1292}, {1420, 1313},
							{1487, 1245}, {1484, 907}, {1221, 608}, 
							{1196, 215}, {1110, 136}, {997, 193}, 
							{988, 319}, {694,338}, {517, 175}, {308,147}, 
							{155,270}, {141,1216}, {203,1316}, {464,1307}, 
							{528,1239}};
	
	wx = waypoints[n][0];
    wy = waypoints[n][1];
	
	beta = angulo-atan2(wx-x,-wy+y); // nuevo angulo de direccion

    if (sin(beta) < 0) 
	{
		angulo += (velocidad_giro + grip)*velocidad; 
	}	
	else 
	{
		angulo -= (velocidad_giro + grip)*velocidad;
	}

	if (((x-wx)*(x-wx)+(y-wy)*(y-wy))<50*50)
	{
		n = (n+1)%21;
		
	}
	
}

void CarAI::moverAuto()
{
	x += sin(angulo) * velocidad;
	y -= cos(angulo) * velocidad;
}

void CarAI::set_Position()
{
	// traslacion de nuestro auto
	spr_car.setPosition(x, y);

	//transformo radianes a grados sexagesimales:
	spr_car.setRotation(angulo*180/3.141593);
}


// posicion inicial del auto CPU
void CarAI::startPosition(float xi, float yi)
{
	x = xi;
	y = yi;
}

// setea la posicion en X
void CarAI::setX(float x1)
{
	x += x1;
}


// setea la posicion en Y
void CarAI::setY(float y1)
{
	y += y1;
}

// devuelve la posicion en X
float CarAI::getX()
{
	return x;
}

// devuelve la posicion en Y
float CarAI::getY()
{
	return y;
}


// setea la posicion en X
void CarAI::setVelocidad(float v1)
{
	velocidad = v1;
}
