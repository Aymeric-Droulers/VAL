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

// Déclaration de la classe Rame
class Rame {
private:
    int numero; // Numéro de la rame
    Troncon* tronconActuel; // Troncon actuel (peut être nullptr si non spécifié)
    int positionTroncon; // Distance parcourue sur le troncon
    float vitesse; // La vitesse du métro
    int PAX; // Nombre de passagers
    int PaxMontant; // Nombre de personnes qui montent dans le métro 
    int PaxDescendant; // Nombre de personnes qui descendent dans le métro 
    int changePaxCompteur; 
    float posX; // Position x de la Rame
    float posY; // Position y de la Rame
    int status; // 1: acceleration 2: vitesse constante 3: deceleration 4:echange de passagers
public:
    // Constructeur par défaut
    Rame() : tronconActuel(nullptr), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0),changePaxCompteur(0), posX(0), posY(0), status(0) {
    }

    // Constructeur avec Troncon
    Rame(Troncon* troncon) : tronconActuel(troncon), numero(0), positionTroncon(0), vitesse(0), PAX(0), PaxDescendant(0), PaxMontant(0), changePaxCompteur(0), posX(0), posY(0),status(0) {
    }
    // Méthodes pour obtenir les getters : 
    int getNumero();// Méthodes pour obtenir le numéro de la Rame
    Troncon* getTronconActuel();// Méthodes pour obtenir le troncon sur lequel la rame se situe 
    int getPositionTroncon();// Méthodes pour la position sur le troncon de la rame 
    float getVitesse();// Méthodes pour obtenir la vitesse de la rame 
    int getPAX();// Méthodes pour obtenir le nombre de passagers dans la rame 
    float getPosX() { return this->posX; }// Méthodes pour obtenir la position x de la rame
    float getPosY() { return this->posY; }// Méthodes pour obtenir la position y de la rame
    int getStatus() { return this->status; }// Méthodes pour obtenir le status de la rame 
    int getPaxMontant() { return this->PaxMontant; }// Méthodes pour obtenir le nombre de passgers qui vont monter dans la rame 
    int getPaxDescendant() { return this->PaxDescendant; }// Méthodes pour obtenir le nombre de passgers qui vont descendre dans la rame 
    int getChangePaxCompteur() { return this->changePaxCompteur; } 

    // Méthodes pour définir les setters : 
    void setNumero(int Nnumero);//Méthode pour modifier le numéro de la rame 
    void setTronconActuel(Troncon* troncon);//Méthode pour modifier le troncon sur lequel se trouve la rame
    void setPositionTroncon(int position);//Méthode pour modifier la position de la rame 
    void setVitesse(float nVitesse);//Méthode pour modifier la vitesse de la rame 
    void setPAX(int nPax);//Méthode pour modifier le nombre de passagers présent dans la rame 
    void setPosX(float posX);//Méthode pour modifier la position x de la rame 
    void setPosY(float posY);//Méthode pour modifier la positiion y de la rame 
    void setStatus(int status);//Méthode pour modifier le status de la rame 
    void setChangePaxCompteur(int n){ this->changePaxCompteur = n; }


    bool security(Troncon& troncon, Rame rame);//Fonction qui gére la sécurité de la rame, la distance avec la rame devant 
    void gesVitesse();//Fonction pour la gestion de la vitesse de la rame 
    void gesPosition();//Fonction pour la gestion de la position de la rame 
    void changeTroncon();//Fonction qui permet de modifier le troncon sur lequel se trouve la rame, il permet de passer au troncon suivant
    void gesPassagers();//Fonction qui permet de gérer les passagers 
};