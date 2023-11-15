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
	int setNumero(int &numero);
	int setTroncon_actuelle( Troncon &troncon);
	int setPosition_troncon( Troncon& troncon);
	float setVitesse( int &vitesse);
	int setPAX( int &Pax);
};