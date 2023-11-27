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
    Troncon* tronconActuel; // Troncon actuel (peut �tre nullptr si non sp�cifi�)
    int positionTroncon; // Distance parcourue sur le troncon
    float vitesse; // La vitesse du m�tro
    int PAX; // Nombre de passagers

public:
    // Constructeur par d�faut
    Rame() : tronconActuel(nullptr), numero(0), positionTroncon(0), vitesse(0), PAX(0) {
    }

    // Constructeur avec Troncon
    Rame(Troncon* troncon) : tronconActuel(troncon), numero(0), positionTroncon(0), vitesse(0), PAX(0) {
    }

    int getNumero();
    Troncon* getTronconActuel();
    int getPositionTroncon();
    float getVitesse();
    int getPAX();

    void setNumero(int Nnumero);
    void setTronconActuel(Troncon* troncon);
    void setPositionTroncon(int position);
    void setVitesse(float nVitesse);
    void setPAX(int nPax);
};