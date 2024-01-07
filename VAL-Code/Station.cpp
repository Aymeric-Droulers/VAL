#include "Station.h"
#include <random>
using namespace std;

// Constructeur de la classe Station
Station::Station(string nom, bool currentTrain, int pax, bool terminus, bool depart, int PosX, int PosY)
{
    // Initialisation des membres de la classe avec les valeurs pass�es en param�tre
    this->nom = nom;
    this->currentTrain = currentTrain;
    this->PAX_quai = pax;
    this->terminus = terminus;
    this->depart = depart;
    this->posX = PosX;
    this->posY = PosY;
}

// M�thode pour obtenir le nom de la station
string Station::getNom()
{
    return this->nom;
}

// M�thode pour obtenir la pr�sence d'un train en cours � la station
bool Station::getCurrentTrain()
{
    return this->currentTrain;
}

// M�thode pour obtenir le nombre de passagers sur le quai
int Station::getPAX_quai()
{
    return this->PAX_quai;
}

// M�thode pour obtenir la position de la station sur l'axe X
int Station::getPosX()
{
    return this->posX;
}

// M�thode pour obtenir la position de la station sur l'axe Y
int Station::getPosY()
{
    return this->posY;
}

// M�thode pour obtenir si la station est un terminus
bool Station::getTerminus()
{
    return this->terminus;
}

// M�thode pour obtenir si la station est un point de d�part
bool Station::getDepart()
{
    return this->depart;
}

// M�thode pour d�finir le nom de la station
void Station::setNom(string nom)
{
    this->nom = nom;
};

// M�thode pour d�finir la pr�sence d'un train en cours � la station
void Station::setCurrentTrain(bool var)
{
    this->currentTrain = var;
};

// M�thode pour d�finir le nombre de passagers sur le quai
void Station::setPAX_quai(int nombre)
{
    this->PAX_quai = nombre;
};

// M�thode pour d�finir si la station est un terminus
void Station::setTerminus(bool var)
{
    this->terminus = var;
};

// M�thode pour d�finir si la station est un point de d�part
void Station::setDepart(bool var)
{
    this->depart = var;
};

// M�thode pour d�finir la position de la station sur l'axe X
void Station::setPosX(int posX)
{
    this->posX = posX;
}

// M�thode pour d�finir la position de la station sur l'axe Y
void Station::setPosY(int posY)
{
    this->posY = posY;
}

// M�thode pour d�finir la position de la station sur les axes X et Y simultan�ment
void Station::setPosXY(int posX, int posY)
{
    this->posX = posX;
    this->posY = posY;
}
