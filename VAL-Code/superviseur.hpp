#pragma once
#include <mutex>
#include "Rame.hpp"
#include "Ligne.hpp"
#include "Station.h"

class Superviseur {
public:
    vector<Rame*> listeRames;
    vector<Ligne*> listeLignes;
    vector<Station*> listeStation;

};
