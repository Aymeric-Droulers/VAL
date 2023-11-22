#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include "troncon.hpp"


using namespace std;

class Rame {
private:
	int numero; // Num�ro de la rame
	int troncon_actuelle; //Station actuelle
	int position_troncon; // Prochaine station
	float vitesse; //La vitesse du m�tro
	int PAX; //Nombre de passagers

public: 
	Rame(int num, int troncon, int pos = 0, int vit = 0, int pass = 0)
		: numero(num), troncon_actuelle(troncon), position_troncon(pos), vitesse(vit), PAX(pass) {
	}
	int getNumero(); 
	int getTroncon_actuelle(); 
	int getPosition_troncon(); 
	float getVitesse(); 
	int getPAX(); 

	int setNumero(int Nnumero);
	int setTroncon_actuelle(int a);
	int setPosition_troncon(int a);
	float setVitesse(int nVitesse);
	int setPAX(int nPax);
};