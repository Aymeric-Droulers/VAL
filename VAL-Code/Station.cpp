#include "Station.h"
#include <random>
using namespace std;

Station::Station(string nom, bool currentTrain, int pax, bool terminus, bool depart,int PosX, int PosY)
{
	this->nom = nom;
	this->currentTrain = currentTrain;
	this->PAX_quai = pax;
	this->terminus = terminus;
	this->depart = depart;
	this->posX = PosX;
	this->posY = PosY;
}

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

void Station::setNom(string nom) {
	this->nom = nom;
};
void Station::setCurrentTrain(bool var) {
	this->currentTrain = var;
}; 
void Station::setPAX_quai(int nombre) {
	this->PAX_quai = nombre;
}; 
void Station::setTerminus(bool var) {
	this->terminus = var;
}; 
void Station::setDepart(bool var) {
	this->depart = var;
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