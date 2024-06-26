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

bool Rame::security(Troncon& troncon, Rame rame) {
    int nombre = static_cast<int>(troncon.getRamesSurLigne().size());
    vector <Rame> liste = {};
    for (size_t i = 0; i < nombre; i++)
    {
        if (troncon.getRamesSurLigne()[i].getPositionTroncon() > rame.getPositionTroncon() and troncon.getRamesSurLigne()[i].getNumero() != rame.getNumero()) {
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
        while (rame1.getPositionTroncon() - rame.getPositionTroncon() < 200) {
            return true;
        }
    }
    return false;
};

void Rame::gesVitesse() {
    int n = 0;
    Troncon* tronconact = this->getTronconActuel();
    if (tronconact->getRamesSurLigne().size() == 1) {
        if (this->getVitesse() <= 50) {
            if (this->getPositionTroncon() < (static_cast<float>(2) / 5) * tronconact->getTailleTroncon()) {// phase d'accélération sur 1/3 du troncon
                this->setStatus(1);
                if (this->getPositionTroncon() < (static_cast<float>(1) / 10) * tronconact->getTailleTroncon()) {
                    this->setVitesse(10);
                }
                else {
                    if (tronconact->getStationFin().getCurrentTrain() == false) {
                        if (this->getVitesse() < 20) {
                            this->setVitesse(static_cast<float>(this->getVitesse() * 1.3));
                        }
                        else {
                            this->setVitesse(static_cast<float>(this->getVitesse() * 1.01));
                        }
                    }
                }
            }
            if(this->getPositionTroncon() > (static_cast<float>(2) / 5) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                this->setStatus(2);
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    this->setVitesse(this->getVitesse() * 1);
                }
            }
            
        }
        if (this->getPositionTroncon() > (static_cast<float>(4) / 5) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
            this->setStatus(3);
            if (tronconact->getStationFin().getCurrentTrain() == false) {
                if (tronconact->getTailleTroncon() - this->getPositionTroncon() > 0) {
          
                    if (this->getVitesse() <= 10) {
                        this->setVitesse(10);
                    }
                    else {
                        this->setVitesse(static_cast<float>(this->getVitesse() * 0.95));
                    }
                }
                else {
                    this->setVitesse(0);
                 //   this->changeTroncon();
                }
                n++;
            }
        }
    }
    else {
        if (this->getVitesse() <= 50 and security(*tronconact, *this) == true) {
            if (this->getPositionTroncon() < (static_cast<float>(2) / 5) * tronconact->getTailleTroncon()) {// phase d'accélération sur 1/3 du troncon
                this->setStatus(1);
                if (this->getPositionTroncon() < (static_cast<float>(1) / 10) * tronconact->getTailleTroncon()) {
                    this->setVitesse(10);
                }
                else {
                    if (tronconact->getStationFin().getCurrentTrain() == false) {
                        if (this->getVitesse() < 20) {
                            this->setVitesse(static_cast<float>(this->getVitesse() * 1.3));
                        }
                        else {
                            this->setVitesse(static_cast<float>(this->getVitesse() * 1.01));
                        }
                    }
                }
            }
            if (this->getPositionTroncon() > (static_cast<float>(2) / 5) * tronconact->getTailleTroncon()) {// phase vitesse constante sur 1/3 du troncon
                this->setStatus(2);
                if (tronconact->getStationFin().getCurrentTrain() == false) {
                    this->setVitesse(this->getVitesse() * 1);
                }
            }
            
        }
        if (this->getPositionTroncon() > (1 / 5) * tronconact->getTailleTroncon()) {// phase déceleration
            this->setStatus(3);
            if (tronconact->getStationFin().getCurrentTrain() == false) {
                if (tronconact->getTailleTroncon() - this->getPositionTroncon() > 0) {

                    if (this->getVitesse() <= 10) {
                        this->setVitesse(10);
                    }
                    else {
                        this->setVitesse(static_cast<float>(this->getVitesse() * 0.95));
                    }
                }
                else {
                    this->setVitesse(0);
                  //  this->changeTroncon();
                }
                n++;
            }
        }
    }
   
};


void Rame::gesPosition() {
    this->setPositionTroncon(static_cast<int>(this->getPositionTroncon() + 0.1 *getVitesse()));
}

void Rame::changeTroncon(){
    this->tronconActuel->removeRameSurLigne(this->getNumero());
    this->setTronconActuel(this->tronconActuel->getTronconSuivant());
    this->tronconActuel->addRameSurTroncon(*this);
    this->setPositionTroncon(0);
    this->setVitesse(1);
}

void Rame::setStatus(int status) {
    this->status = status;
}

void Rame::gesPassagers(){

    int descPax = 0;
    if (this->getPAX() != 0) {
        descPax = rand() % this->getPAX() + 1;
    }
    int montePax = 0;
    if (this->getTronconActuel()->getStationFin().getPAX_quai() > 0) {
        montePax = rand() % this->getTronconActuel()->getStationFin().getPAX_quai();
    }
 

    if (this->getPAX() - descPax + montePax > 50) { //si il y a trop de passagers qui veulent monter
        montePax = 50 - this->getPAX() + descPax;
    }

    if (this->getTronconActuel()->getStationFin().getTerminus() == true) {
        descPax = this->getPAX();
    }

    this->PaxDescendant = descPax;
    this->PaxMontant = montePax;
    this->setChangePaxCompteur(0);
    this->setStatus(4);

}