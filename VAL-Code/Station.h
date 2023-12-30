#pragma once
#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <iostream>
#include <string>


using namespace std;

class Station {
private:
	string nom; // Nom de la station
	bool currentTrain; // Indique la presence d'un metro 
	int PAX_quai; //Nombre de personnes sur le quai 
	bool terminus; // Si la fin de la ligne
	bool depart; // Indique au métro s'il peut partir  
	int posX;
	int posY;
public:
	Station(string nom, bool currentTrain, int pax, bool terminus, bool depart, int PosX=0, int PosY=0);
	string getNom();
	bool getCurrentTrain(); // Indique la presence d'un metro 
	int getPAX_quai(); //Nombre de personnes sur le quai 
	bool getTerminus(); // Si la fin de la ligne
	bool getDepart();
	int getPosX();
	int getPosY();

	void setNom(string nom);
	void setCurrentTrain(bool var); 
	void setPAX_quai(int nombre); 
	void setTerminus(bool var);
	void setDepart(bool var);

	void setPosX(int x);
	void setPosY(int y);
	void setPosXY(int x, int y);
};