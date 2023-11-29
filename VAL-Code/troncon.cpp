#include <iostream>
#include <algorithm>
#include "Rame.hpp"
#include "troncon.hpp"


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

void Troncon::setTronconSuivant(std::shared_ptr<Troncon>& troncon)
{
	this->tronconSuivant = troncon;
}
