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

int Station::getPosX()
{
	return this->posX;
}
int Station::getPosY()
{
	return this->posY;
}

bool Station::getTerminus()
{
	return this->terminus;
}

bool Station::getDepart()
{
	return this->depart;
}

void Station::setNom(string nom) 
{

};
void Station::setCurrentTrain(bool var) 
{
}; 
void Station::setPAX_quai(int nombre) 
{
}; 
void Station::setTerminus(bool var) {
}; 
void Station::setDepart(bool var) {
}; 

void Station::setPosX(int posX) {
	this->posX = posX;
}

void Station::setPosY(int posY) {
	this->posY = posY;
}

void Station::setPosXY(int posX, int posY) {
	this->posX = posX;
	this->posY = posY;
}