#pragma once 

#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>

using namespace std;

// Déclaration de la classe Station
class Station {
private:
    string nom;          // Nom de la station
    bool currentTrain;   // Indique la présence d'un métro
    int PAX_quai;        // Nombre de personnes sur le quai
    bool terminus;       // Si la station est en terminus (fin de la ligne)
    bool depart;         // Indique au métro s'il peut partir
    int posX;            // Position de la station sur l'axe X
    int posY;            // Position de la station sur l'axe Y

public:
    // Constructeur de la classe Station
    // Paramètres par défaut pour la position sur les axes X et Y
    Station(string nom, bool currentTrain, int pax, bool terminus, bool depart, int PosX = 0, int PosY = 0);

    // Méthodes pour obtenir les getters
    string getNom(); // Méthodes pour obtenir le nom de la  station
    bool getCurrentTrain(); // Méthodes pour savoir s'il y a un métro dans la station
    int getPAX_quai(); // Méthodes pour obtenir le nombre de personnes dans la station  
    bool getTerminus(); // Méthodes pour savoir si la station est le terminus de la ligne 
    bool getDepart(); // Méthodes pour savoir si le métro est autorisé a partir 
    int getPosX(); // Méthodes pour obtenir la position en x de la station 
    int getPosY(); // Méthodes pour obtenir la position en y de la station 

    // Méthodes pour définir les setters
    void setNom(string nom); //Méthode pour modifier le nom d'une station
    void setCurrentTrain(bool var); //Méthode pour modifier la présence d'un métro dans la station
    void setPAX_quai(int nombre); //Méthode pour modifier le nombre de personnes présentent dans la station
    void setTerminus(bool var); // Méthode pour modifier si la station est le terminus de la ligne
    void setDepart(bool var);  // Méthode pour modifier le métro peut partir de la station 
    void setPosX(int x); // Méthode pour modifier la position x de la station 
    void setPosY(int y); // Méthode pour modifier la position y de la station 

    // Méthode pour définir la position de la station sur les axes X et Y simultanément
    void setPosXY(int x, int y);
};
