#include "Rame.hpp"

// Méthode pour obtenir le numéro de la rame
int Rame::getNumero()
{
    return this->numero;
}

// Méthode pour obtenir le tronçon actuel de la rame
Troncon* Rame::getTronconActuel()
{
    return this->tronconActuel;
}

// Méthode pour obtenir la position de la rame sur le tronçon
int Rame::getPositionTroncon()
{
    return this->positionTroncon;
}

// Méthode pour obtenir la vitesse de la rame
float Rame::getVitesse()
{
    return this->vitesse;
}

// Méthode pour obtenir le nombre de passagers dans la rame
int Rame::getPAX()
{
    return this->PAX;
}

// Méthode pour définir le numéro de la rame
void Rame::setNumero(int Nnumero)
{
    this->numero = Nnumero;
}

// Méthode pour définir le tronçon actuel de la rame
void Rame::setTronconActuel(Troncon* troncon)
{
    this->tronconActuel = troncon;
}

// Méthode pour définir la position de la rame sur le tronçon
void Rame::setPositionTroncon(int position)
{
    this->positionTroncon = position;
}

// Méthode pour définir la vitesse de la rame
void Rame::setVitesse(float nVitesse)
{
    this->vitesse = nVitesse;
}

// Méthode pour définir le nombre de passagers dans la rame
void Rame::setPAX(int nPax)
{
    this->PAX = nPax;
}

// Méthode pour définir la position de la rame sur l'axe X
void Rame::setPosX(float posX) {
    this->posX = posX;
}

// Méthode pour définir la position de la rame sur l'axe Y
void Rame::setPosY(float posY) {
    this->posY = posY;
}

//Méthode qui permet de vérifier que la distance de sécurité entre les rames est repescté 
bool Rame::security(Troncon& troncon, Rame rame) {
    int nombre = static_cast<int>(troncon.getRamesSurLigne().size());// Je regarder combien il y a de rame sur le troncon
    vector <Rame> liste = {};
    for (size_t i = 0; i < nombre; i++)// Je les trie pour savoir lequel est devant la rame que je sécurise
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
    if (liste.size() == 0) {//Si il n'y pas de troncon sur la rame je n'ai aucune raison de m'inquièter 
        return true;
    }
    else {//Sinon je vérifie qu'il y ai toujours la distance de sécurité entre les deux rames
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
    this->tronconActuel->removeRameSurLigne(this->getNumero());// Récupération du numéro de la rame 
    this->setTronconActuel(this->tronconActuel->getTronconSuivant());//Définitiion du troncon par le troncon suivant
    this->tronconActuel->addRameSurTroncon(*this);//Ajout de la rame dans le troncon
    this->setPositionTroncon(0);//Réinitialisation de la position de la rame sur troncon 
    this->setVitesse(1);//Définition de la vitesse de la rame 
}

void Rame::setStatus(int status) {
    this->status = status;
}

void Rame::gesPassagers(){

    int descPax = 0; // Définition d'un entier qui contient le nombre de passagers qui vont descendre du métro 
    if (this->getPAX() != 0) {// Si le métro n'est pas vide 
        descPax = rand() % this->getPAX() + 1; // Calcul du pourcentage de personne qui vont descendre de manière aléatoire 
    }
    int montePax = 0; // Définition d'un entier qui contient le nombre de passagers qui vont monter dans le métro 
    if (this->getTronconActuel()->getStationFin().getPAX_quai() > 0) {//Si la station que va rejoindre le métro n'est pas vide
        montePax = rand() % this->getTronconActuel()->getStationFin().getPAX_quai();// Calcul du pourcentage de personne qui vont monter de manière aléatoire 
    }
 

    if (this->getPAX() - descPax + montePax > 50) { //si il y a trop de passagers qui veulent monter
        montePax = 50 - this->getPAX() + descPax;
    }

    if (this->getTronconActuel()->getStationFin().getTerminus() == true) {// Si le métro zrrive au termminus de la ligne tout les passagers descendent
        descPax = this->getPAX();
    }

    this->PaxDescendant = descPax;//Modification de la valeur des passagers descendant 
    this->PaxMontant = montePax;//Modification de la valeur des passagers montant
    this->setChangePaxCompteur(0);
    this->setStatus(4);// Changement de status 

}