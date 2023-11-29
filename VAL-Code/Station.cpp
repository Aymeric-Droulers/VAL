#include "Station.h"
using namespace std;

string Station::getNom()
{
	return this->nom;
}

bool Station::getCurrentTrain()
{
	return this->currentTrain;
}

int Station::getPAX_quai()
{
	return this->PAX_quai;
}

bool Station::getTerminus()
{
	return this->terminus;
}

bool Station::getDepart()
{
	return this->depart;
}

void Station::setNom(string Nom) 
{
	this->nom = Nom;
};
void Station::setCurrentTrain(bool var) 
{
	this->currentTrain = var;
}; 
void Station::setPAX_quai(int nombre) 
{
	this->PAX_quai = nombre;
}; 
void Station::setTerminus(bool var) 
{
	this->terminus = var;
}; 
void Station::setDepart(bool var) 
{
	this->depart = var;
}; 
