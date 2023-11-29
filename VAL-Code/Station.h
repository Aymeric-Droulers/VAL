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
	bool depart; // Indique au m�tro s'il peut partir  
public:
	Station(string nom, bool presentTrain, int nbrpersonnes = 0, bool fin, bool partir)
		: nom(nom), currentTrain(presentTrain), PAX_quai(nbrpersonnes), terminus(fin), depart(partir)) {
	}
	string getNom();
	bool getCurrentTrain(); // Indique la presence d'un metro 
	int getPAX_quai(); //Nombre de personnes sur le quai 
	bool getTerminus(); // Si la fin de la ligne
	bool getDepart(); // Indique au m�tro s'il peut partir  

	void setNom(string nom);
	void setCurrentTrain(bool var); // Indique la presence d'un metro 
	void setPAX_quai(int nombre); //Nombre de personnes sur le quai 
	void setTerminus(bool var); // Si la fin de la ligne
	void setDepart(bool var); // Indique au m�tro s'il peut partir  

};