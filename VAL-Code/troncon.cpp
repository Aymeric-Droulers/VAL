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