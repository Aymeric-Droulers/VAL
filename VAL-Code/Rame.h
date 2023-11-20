#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>

#include "troncon.hpp"

#include "Station.h"
using namespace std;

class Rame {
private:
	int numero; // Num�ro de la rame
	Troncon tronconActuel; //Troncon actuel
	int positionTroncon; // Distance parcourue sur le troncon
	float vitesse;//La vitesse du m�tro
	int PAX; //Nombre de passagers

public: 
	Rame() {
		numero = 0;
		tronconActuel;
		positionTroncon = 0;
		vitesse = 0;
		PAX = 0;
	}
	int getNumero(); 
	Troncon getTronconActuel(); 
	int getPositionTroncon(); 
	float getVitesse(); 
	int getPAX(); 

	void setNumero(int Nnumero);
	void setTronconActuel(Troncon& troncon);
	void setPositionTroncon(int postion);
	void setVitesse(float nVitesse);
	void setPAX(int nPax);
};