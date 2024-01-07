#pragma once

#include "Station.h"
#include "troncon.hpp"


class Ligne {
private:
	string nomLigne;
	vector<Station*> listeStation;
	vector<Troncon*>listeTroncon;

public:
	//geters
	string getNomLigne() { return this->nomLigne; }
	vector<Station*> getListeStation() { return this->listeStation; }
	vector<Troncon*> getListeTroncon() { return this->listeTroncon; }

	void setNomLigne(string nomLigne) { this->nomLigne = nomLigne; }
	void setListeStation(vector<Station*> listeStation) { this->listeStation = listeStation; }
	void setListeTroncon(vector<Troncon*> listeTroncon) { this->listeTroncon = listeTroncon; }
};
