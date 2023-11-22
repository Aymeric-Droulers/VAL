#include "Rame.hpp"




int Rame::getNumero()
{
	return this->numero;
}

Troncon* Rame::getTronconActuel()
{
	return this->tronconActuel;
}

int Rame::getPositionTroncon()
{
	return this->positionTroncon;
}

float Rame::getVitesse()
{
	return this->vitesse;
}

int Rame::getPAX()
{
	return this->PAX;
}


void Rame::setNumero(int Nnumero) 
{
	this->numero = Nnumero;
}
void Rame::setTronconActuel(Troncon* troncon)
{
	this->tronconActuel = troncon;
};
void Rame::setPositionTroncon(int position) 
{
	this->positionTroncon = position;
};
void Rame::setVitesse(float nVitesse) 
{
	this->vitesse = nVitesse;
};
void Rame::setPAX(int nPax) 
{
	this->PAX = nPax;
};