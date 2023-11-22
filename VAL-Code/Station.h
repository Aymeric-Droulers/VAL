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
public:
	string getNom();
	bool getCurrentTrain(); // Indique la presence d'un metro 
	int getPAX_quai(); //Nombre de personnes sur le quai 
	bool getTerminus(); // Si la fin de la ligne
	bool getDepart(); // Indique au métro s'il peut partir  

	string setNom(string nom);
	bool setCurrentTrain(bool var); // Indique la presence d'un metro 
	int setPAX_quai(int nombre); //Nombre de personnes sur le quai 
	bool setTerminus(bool var); // Si la fin de la ligne
	bool setDepart(bool var); // Indique au métro s'il peut partir  

	bool getCurrentTrain(); 
	int getPAX_quai();  
	bool getTerminus(); 
	bool getDepart();  
};