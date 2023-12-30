#include "Rame.hpp"




int Rame::getNumero()
{
	return this->numero;
}

Troncon* Rame::getTronconActuel()
{
	return this->tronconActuel;
}

int Rame::getPositionTroncon()
{
	return this->positionTroncon;
}

float Rame::getVitesse()
{
	return this->vitesse;
}

int Rame::getPAX()
{
	return this->PAX;
}


void Rame::setNumero(int Nnumero) 
{
	this->numero = Nnumero;
}
void Rame::setTronconActuel(Troncon* troncon)
{
	this->tronconActuel = troncon;
};
void Rame::setPositionTroncon(int position) 
{
	this->positionTroncon = position;
};
void Rame::setVitesse(float nVitesse) 
{
	this->vitesse = nVitesse;
};
void Rame::setPAX(int nPax) 
{
	this->PAX = nPax;
};


void Rame::setPosX(float posX) {
	this->posX = posX;
}
void Rame::setPosY(float posY) {
	this->posY = posY;
}

bool Rame::security(Troncon& troncon) {
    int nombre = troncon.getRamesSurLigne().size();
    vector <Rame> liste = {};
    for (size_t i = 0; i < nombre; i++)
    {
        if (troncon.getRamesSurLigne()[i].getPositionTroncon() > this->getPositionTroncon() and troncon.getRamesSurLigne()[i].getNumero() != this->getNumero()) {
            for (size_t j = 0; j < liste.size(); j++)
            {
                if (troncon.getRamesSurLigne()[i].getPositionTroncon() < troncon.getRamesSurLigne()[j].getPositionTroncon()) {
                    liste.push_back(troncon.getRamesSurLigne()[i]);
                    swap(liste[i], liste[j]);
                }

            }
        }
    }
    if (liste.size() == 0) {
        return true;
    }
    else {
        Rame rame1 = liste[0];
        while (rame1.getPositionTroncon() - this->getPositionTroncon() < 200) {
            return true;
        }
        this->setVitesse((rame1.getVitesse() - 5));
    }

};

void Rame::gesVitesse(Rame rame) {
    int n = 0;
    Troncon* tronconact = rame.getTronconActuel();
    tronconact->addRameSurTroncon(rame);
    Troncon* prochain = &tronconact->getTronconSuivant();
    if (tronconact->getRamesSurLigne().size() == 1) {
        while (rame.getVitesse() <= 50) {
            while (rame.getPositionTroncon() < (1 / 3) * tronconact->getTailleTroncon()) {// phase d'accélération sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    rame.setVitesse(rame.getVitesse() * 1.3);
                }
            }
            while (rame.getPositionTroncon() > (1 / 3) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    rame.setVitesse(rame.getVitesse() * 1);
                }
            }
            while (rame.getPositionTroncon() > (2 / 3) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    while (tronconact->getTailleTroncon() - rame.getPositionTroncon() > 0) {
                        rame.setVitesse(rame.getVitesse() * 0.5);
                    }
                    rame.setVitesse(0);
                    n++;
                }
            }
        }
    }
    else {
        while (rame.getVitesse() <= 50 and security(*tronconact) == true) {
            while (rame.getPositionTroncon() < (1 / 3) * tronconact->getTailleTroncon()) {// phase d'accélération sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    rame.setVitesse(rame.getVitesse() * 1.3);
                }
            }
            while (rame.getPositionTroncon() > (1 / 3) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    rame.setVitesse(rame.getVitesse() * 1);
                }
            }
            while (rame.getPositionTroncon() > (2 / 3) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    while (tronconact->getTailleTroncon() - rame.getPositionTroncon() > 0) {
                        rame.setVitesse(rame.getVitesse() * 0.5);
                    }
                    rame.setVitesse(0);
                    n++;
                }
            }
        }
    }
    //Changement de troncon
    rame.setTronconActuel(prochain);
    tronconact->addRameSurTroncon(rame);
    void setRamesSurTroncon(const std::vector<Rame>);
    return gesVitesse(rame);

};