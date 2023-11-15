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

