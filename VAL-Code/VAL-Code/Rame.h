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
	int getNumero(); // Numéro de la rame
	int getTroncon_actuelle(); //Station actuelle
	int getPosition_troncon(); // Prochaine station
	float getVitesse(); //La vitesse du métro
	int getPAX(); //Nombre de passagers

	int Rame::setNumero(int& numero);
	int Rame::setTroncon_actuelle(Troncon& troncon);
	int Rame::setPosition_troncon(Troncon& troncon);
	float Rame::setVitesse(int& vitesse);
	int Rame::setPAX(int& Pax);
};