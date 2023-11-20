#include "troncon.hpp"
#include <iostream>


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

	auto elemToRemove = this->ramesSurTroncon.begin();
	while (this->ramesSurTroncon[elemToRemove].numero != rameId || ramesSurTroncon - 1 == this->ramesSurTroncon.end()) {
		elemToRemove += 1;
	}
	if (this->ramesSurTroncon[elemToRemove].numero == rameId) {
		this->ramesSurTroncon.erase(elemToRemove);
	}
	else {
		cout << "Une erreur c'est produite" << endl;
	}
	
}