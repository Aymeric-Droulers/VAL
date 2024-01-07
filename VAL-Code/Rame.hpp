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

// D�claration de la classe Rame
class Rame {
private:
    int numero; // Num�ro de la rame
    Troncon* tronconActuel; // Troncon actuel (peut �tre nullptr si non sp�cifi�)
    int positionTroncon; // Distance parcourue sur le troncon
    float vitesse; // La vitesse du m�tro
    int PAX; // Nombre de passagers
    int PaxMontant; // Nombre de personnes qui montent dans le m�tro 
    int PaxDescendant; // Nombre de personnes qui descendent dans le m�tro 
    int changePaxCompteur; 
    float posX; // Position x de la Rame
    float posY; // Position y de la Rame
    int status; // 1: acceleration 2: vitesse constante 3: deceleration 4:echange de passagers
public:
    // Constructeur par d�faut
    Rame() : tronconActuel(nullptr), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0),changePaxCompteur(0), posX(0), posY(0), status(0) {
    }

    // Constructeur avec Troncon
    Rame(Troncon* troncon) : tronconActuel(troncon), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0), changePaxCompteur(0), posX(0), posY(0),status(0) {
    }
    // M�thodes pour obtenir les getters : 
    int getNumero();// M�thodes pour obtenir le num�ro de la Rame
    Troncon* getTronconActuel();// M�thodes pour obtenir le troncon sur lequel la rame se situe 
    int getPositionTroncon();// M�thodes pour la position sur le troncon de la rame 
    float getVitesse();// M�thodes pour obtenir la vitesse de la rame 
    int getPAX();// M�thodes pour obtenir le nombre de passagers dans la rame 
    float getPosX() { return this->posX; }// M�thodes pour obtenir la position x de la rame
    float getPosY() { return this->posY; }// M�thodes pour obtenir la position y de la rame
    int getStatus() { return this->status; }// M�thodes pour obtenir le status de la rame 
    int getPaxMontant() { return this->PaxMontant; }// M�thodes pour obtenir le nombre de passgers qui vont monter dans la rame 
    int getPaxDescendant() { return this->PaxDescendant; }// M�thodes pour obtenir le nombre de passgers qui vont descendre dans la rame 
    int getChangePaxCompteur() { return this->changePaxCompteur; } 

    // M�thodes pour d�finir les setters : 
    void setNumero(int Nnumero);//M�thode pour modifier le num�ro de la rame 
    void setTronconActuel(Troncon* troncon);//M�thode pour modifier le troncon sur lequel se trouve la rame
    void setPositionTroncon(int position);//M�thode pour modifier la position de la rame 
    void setVitesse(float nVitesse);//M�thode pour modifier la vitesse de la rame 
    void setPAX(int nPax);//M�thode pour modifier le nombre de passagers pr�sent dans la rame 
    void setPosX(float posX);//M�thode pour modifier la position x de la rame 
    void setPosY(float posY);//M�thode pour modifier la positiion y de la rame 
    void setStatus(int status);//M�thode pour modifier le status de la rame 
    void setChangePaxCompteur(int n){ this->changePaxCompteur = n; }


    bool security(Troncon& troncon, Rame rame);//Fonction qui g�re la s�curit� de la rame, la distance avec la rame devant 
    void gesVitesse();//Fonction pour la gestion de la vitesse de la rame 
    void gesPosition();//Fonction pour la gestion de la position de la rame 
    void changeTroncon();//Fonction qui permet de modifier le troncon sur lequel se trouve la rame, il permet de passer au troncon suivant
    void gesPassagers();//Fonction qui permet de g�rer les passagers 
};