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
	int numero; // Numéro de la rame
	int troncon_actuelle; //Station actuelle
	int position_troncon; // Prochaine station
	float vitesse; //La vitesse du métro
	int PAX; //Nombre de passagers

public: 
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