#include "Station.h"
#include <random>
using namespace std;

// Constructeur de la classe Station
Station::Station(string nom, bool currentTrain, int pax, bool terminus, bool depart, int PosX, int PosY)
{
    // Initialisation des membres de la classe avec les valeurs passées en paramètre
    this->nom = nom;
    this->currentTrain = currentTrain;
    this->PAX_quai = pax;
    this->terminus = terminus;
    this->depart = depart;
    this->posX = PosX;
    this->posY = PosY;
}

// Méthode pour obtenir le nom de la station
string Station::getNom()
{
    return this->nom;
}

// Méthode pour obtenir la présence d'un train en cours à la station
bool Station::getCurrentTrain()
{
    return this->currentTrain;
}

// Méthode pour obtenir le nombre de passagers sur le quai
int Station::getPAX_quai()
{
    return this->PAX_quai;
}

// Méthode pour obtenir la position de la station sur l'axe X
int Station::getPosX()
{
    return this->posX;
}

// Méthode pour obtenir la position de la station sur l'axe Y
int Station::getPosY()
{
    return this->posY;
}

// Méthode pour obtenir si la station est un terminus
bool Station::getTerminus()
{
    return this->terminus;
}

// Méthode pour obtenir si la station est un point de départ
bool Station::getDepart()
{
    return this->depart;
}

// Méthode pour définir le nom de la station
void Station::setNom(string nom)
{
    this->nom = nom;
};

// Méthode pour définir la présence d'un train en cours à la station
void Station::setCurrentTrain(bool var)
{
    this->currentTrain = var;
};

// Méthode pour définir le nombre de passagers sur le quai
void Station::setPAX_quai(int nombre)
{
    this->PAX_quai = nombre;
};

// Méthode pour définir si la station est un terminus
void Station::setTerminus(bool var)
{
    this->terminus = var;
};

// Méthode pour définir si la station est un point de départ
void Station::setDepart(bool var)
{
    this->depart = var;
};

// Méthode pour définir la position de la station sur l'axe X
void Station::setPosX(int posX)
{
    this->posX = posX;
}

// Méthode pour définir la position de la station sur l'axe Y
void Station::setPosY(int posY)
{
    this->posY = posY;
}

// Méthode pour définir la position de la station sur les axes X et Y simultanément
void Station::setPosXY(int posX, int posY)
{
    this->posX = posX;
    this->posY = posY;
}
