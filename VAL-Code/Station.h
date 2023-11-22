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
	bool getCurrentTrain(); 
	int getPAX_quai();  
	bool getTerminus(); 
	bool getDepart();  

	string Station::setNom(string Nom);
	bool Station::setCurrentTrain(bool var);  
	int Station::setPAX_quai(int nombre); 
	bool Station::setTerminus(bool var); 
	bool Station::setDepart(bool var);   

};