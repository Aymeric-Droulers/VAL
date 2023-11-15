#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>


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

	int Rame::setNumero(int Nnumero);
	int Rame::setTroncon_actuelle(Troncon& troncon);
	int Rame::setPosition_troncon(Troncon& troncon);
	float Rame::setVitesse(int nVitesse);
	int Rame::setPAX(int nPax);
};