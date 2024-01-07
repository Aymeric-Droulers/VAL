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
    int numero; // Numéro de la rame
    Troncon* tronconActuel; // Troncon actuel (peut être nullptr si non spécifié)
    int positionTroncon; // Distance parcourue sur le troncon
    float vitesse; // La vitesse du métro
    int PAX; // Nombre de passagers
    int PaxMontant;
    int PaxDescendant;
    int changePaxCompteur;
    float posX;
    float posY;
    int status; // 1: acceleration 2: vitesse constante 3: deceleration 4:echange de passagers
public:
    // Constructeur par défaut
    Rame() : tronconActuel(nullptr), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0),changePaxCompteur(0), posX(0), posY(0), status(0) {
    }

    // Constructeur avec Troncon
    Rame(Troncon* troncon) : tronconActuel(troncon), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0), changePaxCompteur(0), posX(0), posY(0),status(0) {
    }

    int getNumero();
    Troncon* getTronconActuel();
    int getPositionTroncon();
    float getVitesse();
    int getPAX();
    float getPosX() { return this->posX; }
    float getPosY() { return this->posY; }
    int getStatus() { return this->status; }
    int getPaxMontant() { return this->PaxMontant; }
    int getPaxDescendant() { return this->PaxDescendant; }
    int getChangePaxCompteur() { return this->changePaxCompteur; }

    void setNumero(int Nnumero);
    void setTronconActuel(Troncon* troncon);
    void setPositionTroncon(int position);
    void setVitesse(float nVitesse);
    void setPAX(int nPax);
    void setPosX(float posX);
    void setPosY(float posY);
    bool security(Troncon& troncon, Rame rame);
    void gesVitesse();
    void gesPosition();
    void changeTroncon();
    void setStatus(int status);

    void setChangePaxCompteur(int n) { this->changePaxCompteur = n; }
    void gesPassagers();
};
