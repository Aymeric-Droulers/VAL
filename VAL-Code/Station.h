#pragma once 

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>

using namespace std;

// D�claration de la classe Station
class Station {
private:
    string nom;          // Nom de la station
    bool currentTrain;   // Indique la pr�sence d'un m�tro
    int PAX_quai;        // Nombre de personnes sur le quai
    bool terminus;       // Si la station est en terminus (fin de la ligne)
    bool depart;         // Indique au m�tro s'il peut partir
    int posX;            // Position de la station sur l'axe X
    int posY;            // Position de la station sur l'axe Y

public:
    // Constructeur de la classe Station
    // Param�tres par d�faut pour la position sur les axes X et Y
    Station(string nom, bool currentTrain, int pax, bool terminus, bool depart, int PosX = 0, int PosY = 0);

    // M�thodes pour obtenir les getters
    string getNom(); // M�thodes pour obtenir le nom de la  station
    bool getCurrentTrain(); // M�thodes pour savoir s'il y a un m�tro dans la station
    int getPAX_quai(); // M�thodes pour obtenir le nombre de personnes dans la station  
    bool getTerminus(); // M�thodes pour savoir si la station est le terminus de la ligne 
    bool getDepart(); // M�thodes pour savoir si le m�tro est autoris� a partir 
    int getPosX(); // M�thodes pour obtenir la position en x de la station 
    int getPosY(); // M�thodes pour obtenir la position en y de la station 

    // M�thodes pour d�finir les setters
    void setNom(string nom); //M�thode pour modifier le nom d'une station
    void setCurrentTrain(bool var); //M�thode pour modifier la pr�sence d'un m�tro dans la station
    void setPAX_quai(int nombre); //M�thode pour modifier le nombre de personnes pr�sentent dans la station
    void setTerminus(bool var); // M�thode pour modifier si la station est le terminus de la ligne
    void setDepart(bool var);  // M�thode pour modifier le m�tro peut partir de la station 
    void setPosX(int x); // M�thode pour modifier la position x de la station 
    void setPosY(int y); // M�thode pour modifier la position y de la station 

    // M�thode pour d�finir la position de la station sur les axes X et Y simultan�ment
    void setPosXY(int x, int y);
};
