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

<<<<<<< HEAD:VAL-Code/VAL-Code/Station.cpp
string Station::setNom(string nom) 
{

};
bool Station::setCurrentTrain(bool var) 
{
}; 
int Station::setPAX_quai(int nombre) 
{
}; 
bool Station::setTerminus(bool var) {
}; 
bool Station::setDepart(bool var) {
=======
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
>>>>>>> main:VAL-Code/Station.cpp
}; 
