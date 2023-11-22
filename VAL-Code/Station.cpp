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

string Station::setNom(string Nom) 
{
	this->nom = Nom;
};
bool Station::setCurrentTrain(bool var) 
{
	this->currentTrain = var;
}; 
int Station::setPAX_quai(int nombre) 
{
	this->PAX_quai = nombre;
}; 
bool Station::setTerminus(bool var) 
{
	this->terminus = var;
}; 
bool Station::setDepart(bool var) 
{
	this->depart = var;
}; 
