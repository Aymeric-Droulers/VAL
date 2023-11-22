#include <iostream>
#include <algorithm>
#include "Rame.hpp"
#include "troncon.hpp"

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