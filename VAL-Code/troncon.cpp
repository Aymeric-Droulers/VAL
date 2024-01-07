#include <iostream>
#include <algorithm>
#include <chrono>
#include "Rame.hpp"
#include "troncon.hpp"

using namespace std;

Troncon::Troncon(Station& stationDebut, Station& stationFin, int distance, std::vector<Point2D> trace)
	: stationDebut(stationDebut), stationFin(stationFin), tailleTroncon(distance), tronconSuivant(nullptr),trace(trace) { // ou une initialisation appropriée pour tronconSuivant
	// Le corps du constructeur (si nécessaire)
}


void Troncon::setStationDebut(const Station& station)
{
	this->stationDebut = station ;
}

void Troncon::setStationFin(const Station& station)
{
	this->stationFin = station;
}

void Troncon::setTailleTroncon(int taille)
{
	if (taille >= 0) {
		this->tailleTroncon = taille;
	}
	std::cout << "Vous ne devez entrer une taille de troncon strictement supérieure à 0" << std::endl;
}

void Troncon::setRamesSurTroncon(const std::vector<Rame> listeRames) {
	this->ramesSurTroncon = listeRames;
}

void Troncon::addRameSurTroncon(const Rame rame) {
	this->ramesSurTroncon.push_back(rame);
}

void Troncon::removeRameSurLigne(int rameId) {
	for (int i = 0; i < this->ramesSurTroncon.size(); i++) {
		if (this->ramesSurTroncon[i].getNumero() == rameId) {
			this->ramesSurTroncon.erase(this->ramesSurTroncon.begin() + i);
		}
	}

	
}

void Troncon::setTronconSuivant(Troncon& troncon)
{
	this->tronconSuivant = &troncon;
}

/*
void Troncon::gesPasssagers(Rame &rame) {
	int temps = 30; // Le temps d'arret minimum du métro
	//si début de ligne
	if (rame.getPositionTroncon() == 0) {
		int v1 = rand() % 10;
		this->getStationDebut().setPAX_quai(v1);
		if (rame.getPAX() + v1 > 50) {
			this->getStationDebut().setPAX_quai((rame.getPAX() + v1) - 50);
			rame.setPAX(50);
		}
		rame.setPAX(v1);
		this->getStationDebut().setPAX_quai(0);
	}
	if (rame.getPositionTroncon() == this->getTailleTroncon()) {
		//terminus
		int v1 = rand() % 10;
		rame.setPAX(0);//rame est vide
		this->getStationFin().setPAX_quai(v1);
	}
	else {
		int v1 = rand() % 10; //valeur pour les personnes dans la station
		int v2 = rand() % 5; //valeur des personnes qui veulent sortir du métro
		this->getStationDebut().setPAX_quai(v1);
		if ((rame.getPAX() + v1)-v2 > 50) {
			this->getStationDebut().setPAX_quai((rame.getPAX() + v1) - 50);
			rame.setPAX(50);
		}
		rame.setPAX(v1);
		this->getStationDebut().setPAX_quai(0);
	}
	temps += (1 * rame.getPAX());
	this_thread::sleep_for(std::chrono::seconds(temps));
}*/