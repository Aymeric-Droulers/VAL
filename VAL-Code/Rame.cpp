#include "Rame.h"

int Rame::getNumero()
{
	return this->numero;
}

int Rame::getTroncon_actuelle()
{
	return this->troncon_actuelle;
}

int Rame::getPosition_troncon()
{
	return this->position_troncon;
}

float Rame::getVitesse()
{
	return this->vitesse;
}

int Rame::getPAX()
{
	return this->PAX;
}


int Rame::setNumero(int Nnumero) 
{
	this->numero = Nnumero;
}

int Rame::setTroncon_actuelle( int a) 
{
	this->troncon_actuelle = a;
};
int Rame::setPosition_troncon(int a)
{
	this->position_troncon =a;
};
float Rame::setVitesse( int nVitesse) 
{
	this->vitesse = nVitesse;
};
int Rame::setPAX(int nPax) 
{
	this->PAX = nPax;
};