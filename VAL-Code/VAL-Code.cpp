// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "Rame.hpp"
#include "Station.h"
#include "troncon.hpp"
#include "superviseur.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace std;

bool Open = true;
int numRames;




float distancePoints(vector<Point2D> lstPoints) {
    float distTotale = 0;
    for (int i = 0; i < lstPoints.size()-1; i++) {
        float diffX = static_cast<float>(lstPoints[i + 1].posX - lstPoints[i].posX);
        float diffY = static_cast<float>(lstPoints[i + 1].posY - lstPoints[i].posY);
        distTotale += (float)sqrt(pow(diffX, 2) + pow(diffY, 2));
    }
    return distTotale;
}


void iteratorRames(Superviseur& reseau) {
    while (Open) {
        for (int iRames = 0; iRames < reseau.listeRames.size(); iRames++) {//pour chaque rame
            Rame* rame = reseau.listeRames[iRames];
            Troncon* troncon = rame->getTronconActuel();
            if (rame->getPositionTroncon() < troncon->getTailleTroncon()) { //si la rame n'est pas a une station
                rame->gesPosition();
                rame->gesVitesse();
            }
            else { //si la rame est a une station
                if (rame->getStatus() != 4) {

                    rame->gesPassagers(); //calcul des passagers a échanger
                }
                else {
                    if (rame->getChangePaxCompteur() < rame->getPaxDescendant()) { //Descente des passagers
                        rame->setChangePaxCompteur(rame->getChangePaxCompteur() + 1);
                        rame->setPAX(rame->getPAX() - 1);
                    }
                    if (rame->getPaxDescendant() <= rame->getChangePaxCompteur() && rame->getChangePaxCompteur() < rame->getPaxMontant()+rame->getPaxDescendant()) {//montée des passagers
                        rame->setChangePaxCompteur(rame->getChangePaxCompteur() + 1);
                        rame->setPAX(rame->getPAX() + 1);
                        Station* station = &rame->getTronconActuel()->getStationFin();
                        station->setPAX_quai(station->getPAX_quai() - 1);
                    }
                    if(rame->getChangePaxCompteur()>=rame->getPaxMontant()+rame->getPaxDescendant()){
                        rame->setChangePaxCompteur(0);
                       
                        rame->changeTroncon();
                    }
                }
            }

          
            
        }

        for (int iStations = 0; iStations < reseau.listeStation.size(); iStations++) {
            Station* station = reseau.listeStation[iStations];
            int isAddingPax = rand() % 80;
            if (isAddingPax == 1) {
                int AddPax = rand() % 3;
                station->setPAX_quai(station->getPAX_quai() + AddPax);
            }
        }

        this_thread::sleep_for(chrono::milliseconds(10));
    }
}



void affichage(sf::RenderWindow &window,Superviseur& reseau) {
    sf::Font font;
    if (!font.loadFromFile(path_font + "arial.ttf"))
    {
        cout << "La police n'a pas pu être chargé";
    }

    sf::Texture textureMetro;
    if (!textureMetro.loadFromFile(path_img+"metro.png"))
    {
        cout << "L'image n'a pas pu être chargé";
    }

    sf::Texture textureMetroFrein;
    if (!textureMetroFrein.loadFromFile(path_img + "metroFrein.png"))
    {
        cout << "L'image n'a pas pu être chargé";
    }

    //Affichage de l'overlay
    sf::Text infos;
    infos.setFont(font);
    infos.setString("Modélisation du VAL\n Commandes:\n Echap: fermer la fenètre \n A: ajouter une rame");
    infos.setCharacterSize(16); // exprimée en pixels, pas en points !
    infos.setFillColor(sf::Color::White);
    infos.setPosition(10.f,10.f);
    window.draw(infos);
    


    //Affichage des tracés des lignes:
    for (int iLignes = 0; iLignes < reseau.listeLignes.size(); iLignes++) {
        Ligne* ligne = reseau.listeLignes[iLignes];
        for (int nTroncon = 0; nTroncon < ligne->getListeTroncon().size(); nTroncon++) {
            Troncon* troncon = ligne->getListeTroncon()[nTroncon];
            sf::VertexArray lines(sf::LinesStrip, troncon->getTrace().size());
            for (size_t i = 0; i < troncon->getTrace().size(); ++i) {
                lines[i].position = sf::Vector2f(static_cast<float>(troncon->getTrace()[i].posX), static_cast<float>(troncon->getTrace()[i].posY));
                lines[i].color = sf::Color::Yellow;
            }
            window.draw(lines);
        }
    }

    //Affichage des stations:
    for (int iStations = 0; iStations < reseau.listeStation.size(); iStations++) {
        Station* station = reseau.listeStation[iStations];

        //cercle

        sf::CircleShape cercleStation(10.f);
        cercleStation.setFillColor(sf::Color(250, 50, 50));
        cercleStation.setPosition(static_cast<float>(station->getPosX()), static_cast<float>(station->getPosY()));
        cercleStation.setOrigin(10.f, 10.f);
        window.draw(cercleStation);

        //texte
        


        sf::Text text;


        text.setFont(font);
        text.setString(station->getNom()+" ("+to_string(station->getPAX_quai())+" PAX)");
        text.setCharacterSize(16); // exprimée en pixels, pas en points !
        text.setFillColor(sf::Color::White);
        text.setPosition(static_cast<float>(station->getPosX()), static_cast<float>(station->getPosY()));
        window.draw(text);
        
    }


    //affichage des rames:
    for (int iRames = 0; iRames < reseau.listeRames.size(); iRames++) {
        Rame* rame = reseau.listeRames[iRames];
        Troncon* troncon = rame->getTronconActuel();
        int position = rame->getPositionTroncon();
        float posPourCent = (static_cast<float>(position) / troncon->getTailleTroncon());
        float distanceTotalePx = distancePoints(troncon->getTrace());
        float positionPx = posPourCent * distanceTotalePx;
        float distParcouruePx = 0;
        int compteurPointsPasses = 0;
        float distanceTroncon = 0;
        bool depasse = false;
        int i = 0;

        while (i < troncon->getTrace().size() - 1 && !depasse) {
            vector<Point2D> morceau = { troncon->getTrace()[i], troncon->getTrace()[i + 1] };
            float distanceMorceau = distancePoints(morceau);
            if (distParcouruePx + distanceMorceau < positionPx) {
                compteurPointsPasses++;
                distParcouruePx += distanceMorceau;
            }
            else {
                depasse = true;
                distanceTroncon = distanceMorceau;
            }
            i++;
        }
        // Calcul de la distance restante sur le tronçon actuel
        float distanceRestanteSurTroncon = positionPx - distParcouruePx;
        float pourcentageSurTroncon = 0;
        if (distanceTroncon != 0) {
            pourcentageSurTroncon = distanceRestanteSurTroncon / distanceTroncon;
        }
        float distanceTronconX = (float)troncon->getTrace()[compteurPointsPasses + 1].posX - (float)troncon->getTrace()[compteurPointsPasses].posX;
        float distanceTronconY = (float)troncon->getTrace()[compteurPointsPasses + 1].posY - (float)troncon->getTrace()[compteurPointsPasses].posY;

        float newPosX = troncon->getTrace()[compteurPointsPasses].posX + (distanceTronconX * pourcentageSurTroncon);
        float newPosY = troncon->getTrace()[compteurPointsPasses].posY + (distanceTronconY * pourcentageSurTroncon);
        
  
        //Affichage
        sf::Sprite shape;
        shape.setTexture(textureMetro);
        if (rame->getStatus()==3) { //si il freine
            shape.setTexture(textureMetroFrein);
        }
        shape.setScale(0.75, 0.75);
        shape.setOrigin(9.75, 13.5);
        shape.setPosition(newPosX, newPosY);
        window.draw(shape);
        sf::Text text;
        text.setFont(font);
        text.setString(to_string(rame->getNumero())+"\n"+ to_string((int)round(rame->getVitesse())) + " km/h"+"\n"+to_string(rame->getPAX())+"pax");
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(16); // exprimée en pixels, pas en points !
        text.setFillColor(sf::Color::White);
        text.setPosition(newPosX,newPosY);
        window.draw(text);
    }

    this_thread::sleep_for(chrono::milliseconds(75));
}

void AddRame(Superviseur& reseau) {
    cout << "NewRame" << endl;
    cout << numRames << endl;
    Rame* rame = new Rame();  // Utilisez new pour créer l'objet Rame sur le tas
    rame->setTronconActuel(reseau.listeLignes[0]->getListeTroncon()[0]);
    rame->setNumero(++numRames);
    rame->setPAX(10);
    rame->setPositionTroncon(0);
    rame->setVitesse(1);
    reseau.listeLignes[0]->getListeTroncon()[0]->addRameSurTroncon(*rame);
    reseau.listeRames.push_back(rame);  // Stockez le pointeur dans le vecteur
}




int main()
{
    // Calcul des coefficients pour la largeur et la hauteur
    float coeff_largeur = 4;  
    float coeff_hauteur = 1.5;  

    //Création ligne
   // gestion présence des personnes 
   //ligne n°1 : 
   //Création des stations : 
   Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, 0, false, true, 383 * coeff_largeur, 466 * coeff_hauteur);
Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, 0, false, false, 389 * coeff_largeur, 446 * coeff_hauteur);
Station Triolo("Triolo", false, 0, false, false, 386 * coeff_largeur, 429 * coeff_hauteur);
Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, 0, false, false, 365 * coeff_largeur, 422 * coeff_hauteur);
Station Pont_de_Bois("Pont de Bois", false, 0, false, false, 360 * coeff_largeur, 405 * coeff_hauteur);
Station Square_Flandres("Square Flandres", false, 0, false, false, 326 * coeff_largeur, 399 * coeff_hauteur);
Station Mairie_dHellemmes("Mairie d'Hellemmes", false, 0, false, false, 316 * coeff_largeur, 397 * coeff_hauteur);
Station Marbrerie("Marbrerie", false, 0, false, false, 299 * coeff_largeur, 388 * coeff_hauteur);
Station Fives("Fives", false, 0, false, false, 286 * coeff_largeur, 378 * coeff_hauteur);
Station Caulier("Caulier", false, 0, false, false, 279 * coeff_largeur, 366 * coeff_hauteur);
Station Gare_Lille_Flandres("Gare Lille-Flandres", false, 0, false, false, 246 * coeff_largeur, 367 * coeff_hauteur);
Station Rihour("Rihour", false, 0, false, false, 232 * coeff_largeur, 370 * coeff_hauteur);
Station République_Beaux_Arts("République - Beaux-Arts", false, 0, false, false, 227 * coeff_largeur, 383 * coeff_hauteur);
Station Gambetta("Gambetta", false, 0, false, false, 209 * coeff_largeur, 400 * coeff_hauteur);
Station Wazemmes("Wazemmes", false, 0, false, false, 208 * coeff_largeur, 410 * coeff_hauteur);
Station Porte_des_Postes("Porte des Postes", false, 0, false, false, 207 * coeff_largeur, 424 * coeff_hauteur);
Station CHU_Centre_Oscar_Lambret("CHU - Centre Oscar-Lambret", false, 0, false, false, 178 * coeff_largeur, 444 * coeff_hauteur);
Station CHU_Eurasanté("CHU - Eurasanté", false, 0, true, false, 183 * coeff_largeur, 457 * coeff_hauteur);
    





    vector<Station*>ligne1Stations = { &CHU_Eurasanté, &CHU_Centre_Oscar_Lambret, &Porte_des_Postes, &Wazemmes, &Gambetta, &République_Beaux_Arts, &Rihour, &Gare_Lille_Flandres, &Caulier, &Fives, &Marbrerie, &Mairie_dHellemmes, &Square_Flandres, &Pont_de_Bois, &Villeneuve_dAscq, &Triolo, &Cité_scientifique, &Quatre_Cantons };
    vector<Troncon*>ligne1Troncons = {};
    vector<Point2D> trace = { Point2D(183 * coeff_largeur, 457 * coeff_hauteur),
                          Point2D(181 * coeff_largeur, 450 * coeff_hauteur),
                          Point2D(178 * coeff_largeur, 444 * coeff_hauteur) };
    Troncon ligne1_CHU_Eurasante(CHU_Eurasanté, CHU_Centre_Oscar_Lambret, 542, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Eurasante);

    trace = { Point2D(178 * coeff_largeur, 444 * coeff_hauteur),
              Point2D(193 * coeff_largeur, 433 * coeff_hauteur),
              Point2D(207 * coeff_largeur, 424 * coeff_hauteur) };
    Troncon ligne1_CHU_Centre_Oscar_Lambert(CHU_Centre_Oscar_Lambret, Porte_des_Postes, 1301, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Centre_Oscar_Lambert);

    trace = { Point2D(207 * coeff_largeur, 424 * coeff_hauteur),
              Point2D(207 * coeff_largeur, 416 * coeff_hauteur),
              Point2D(208 * coeff_largeur, 410 * coeff_hauteur) };
    Troncon ligne1_Porte_des_Postes(Porte_des_Postes, Wazemmes, 505, trace);
    ligne1Troncons.push_back(&ligne1_Porte_des_Postes);

    trace = { Point2D(208 * coeff_largeur, 410 * coeff_hauteur),
              Point2D(208 * coeff_largeur, 404 * coeff_hauteur),
              Point2D(209 * coeff_largeur, 400 * coeff_hauteur) };
    Troncon ligne1_Wazemes(Wazemmes, Gambetta, 430, trace);
    ligne1Troncons.push_back(&ligne1_Wazemes);

    trace = { Point2D(209 * coeff_largeur, 400 * coeff_hauteur),
              Point2D(218 * coeff_largeur, 376 * coeff_hauteur),
              Point2D(227 * coeff_largeur, 383 * coeff_hauteur) };
    Troncon ligne1_Gambetta(Gambetta, République_Beaux_Arts, 928, trace);
    ligne1Troncons.push_back(&ligne1_Gambetta);

    trace = { Point2D(227 * coeff_largeur, 383 * coeff_hauteur),
              Point2D(229 * coeff_largeur, 376 * coeff_hauteur),
              Point2D(232 * coeff_largeur, 370 * coeff_hauteur) };
    Troncon ligne1_Republique(République_Beaux_Arts, Rihour, 534, trace);
    ligne1Troncons.push_back(&ligne1_Republique);

    trace = { Point2D(232 * coeff_largeur, 370 * coeff_hauteur),
              Point2D(232 * coeff_largeur, 370 * coeff_hauteur),
              Point2D(246 * coeff_largeur, 367 * coeff_hauteur) };
    Troncon ligne1_Rhiour(Rihour, Gare_Lille_Flandres, 1240, trace);
    ligne1Troncons.push_back(&ligne1_Rhiour);

    trace = { Point2D(246 * coeff_largeur, 367 * coeff_hauteur),
              Point2D(263 * coeff_largeur, 366 * coeff_hauteur),
              Point2D(279 * coeff_largeur, 366 * coeff_hauteur) };
    Troncon ligne1_Flandres(Gare_Lille_Flandres, Caulier, 754, trace);
    ligne1Troncons.push_back(&ligne1_Flandres);

    trace = { Point2D(279 * coeff_largeur, 366 * coeff_hauteur),
              Point2D(282 * coeff_largeur, 372 * coeff_hauteur),
              Point2D(286 * coeff_largeur, 378 * coeff_hauteur) };
    Troncon ligne1_Caulier(Caulier, Fives, 582, trace);
    ligne1Troncons.push_back(&ligne1_Caulier);

    trace = { Point2D(286 * coeff_largeur, 378 * coeff_hauteur),
              Point2D(292 * coeff_largeur, 383 * coeff_hauteur),
              Point2D(299 * coeff_largeur, 388 * coeff_hauteur) };
    Troncon ligne1_Fives(Fives, Marbrerie, 665, trace);
    ligne1Troncons.push_back(&ligne1_Fives);

    trace = { Point2D(299 * coeff_largeur, 388 * coeff_hauteur),
              Point2D(307 * coeff_largeur, 393 * coeff_hauteur),
              Point2D(316 * coeff_largeur, 397 * coeff_hauteur) };
    Troncon ligne1_Marbrerie(Marbrerie, Mairie_dHellemmes, 567, trace);
    ligne1Troncons.push_back(&ligne1_Marbrerie);

    trace = { Point2D(316 * coeff_largeur, 397 * coeff_hauteur),
              Point2D(321 * coeff_largeur, 398 * coeff_hauteur),
              Point2D(326 * coeff_largeur, 399 * coeff_hauteur) };
    Troncon ligne1_MairiedHellmes(Mairie_dHellemmes, Square_Flandres, 209, trace);
    ligne1Troncons.push_back(&ligne1_MairiedHellmes);

    trace = { Point2D(326 * coeff_largeur, 399 * coeff_hauteur),
              Point2D(346 * coeff_largeur, 401 * coeff_hauteur),
              Point2D(360 * coeff_largeur, 405 * coeff_hauteur) };
    Troncon ligne1_SquareFlandres(Square_Flandres, Pont_de_Bois, 1254, trace);
    ligne1Troncons.push_back(&ligne1_SquareFlandres);

    trace = { Point2D(360 * coeff_largeur, 405 * coeff_hauteur),
              Point2D(363 * coeff_largeur, 413 * coeff_hauteur),
              Point2D(365 * coeff_largeur, 422 * coeff_hauteur) };
    Troncon ligne1_PontDeBois(Pont_de_Bois, Villeneuve_dAscq, 639, trace);
    ligne1Troncons.push_back(&ligne1_PontDeBois);

    trace = { Point2D(365 * coeff_largeur, 422 * coeff_hauteur),
              Point2D(379 * coeff_largeur, 420 * coeff_hauteur),
              Point2D(386 * coeff_largeur, 429 * coeff_hauteur) };
    Troncon ligne1_Villeneuve(Villeneuve_dAscq, Triolo, 919, trace);
    ligne1Troncons.push_back(&ligne1_Villeneuve);

    trace = { Point2D(386 * coeff_largeur, 429 * coeff_hauteur),
              Point2D(389 * coeff_largeur, 438 * coeff_hauteur),
              Point2D(389 * coeff_largeur, 446 * coeff_hauteur) };
    Troncon ligne1_Triolo(Triolo, Cité_scientifique, 642, trace);
    ligne1Troncons.push_back(&ligne1_Triolo);

    trace = { Point2D(389 * coeff_largeur, 446 * coeff_hauteur),
              Point2D(384 * coeff_largeur, 455 * coeff_hauteur),
              Point2D(383 * coeff_largeur, 466 * coeff_hauteur) };
    Troncon ligne1_Cite(Cité_scientifique, Quatre_Cantons, 781, trace);
    ligne1Troncons.push_back(&ligne1_Cite);



    //Le retour 

   
  
   // Quatre_Cantons vers Cité_scientifique (retour)
   // Trace entre les stations Quatre_Cantons et Cité_scientifique
    trace = { Point2D(183 * coeff_largeur, 457 * coeff_hauteur),
              Point2D((181 + 0.01 * 181) * coeff_largeur, (450 + 0.01 * 450) * coeff_hauteur),
              Point2D(178 * coeff_largeur, 444 * coeff_hauteur) };
    Troncon ligne1r_Quatre_Cantons = Troncon(Quatre_Cantons, Cité_scientifique, 781, trace);
    ligne1Troncons.push_back(&ligne1r_Quatre_Cantons);

    // Cité_scientifique vers Triolo (retour)
    // Trace entre les stations Cité_scientifique et Triolo
    trace = { Point2D(178 * coeff_largeur, 444 * coeff_hauteur),
              Point2D((193 + 0.01 * 193) * coeff_largeur, (433 + 0.01 * 433) * coeff_hauteur),
              Point2D(207 * coeff_largeur, 424 * coeff_hauteur) };
    Troncon ligne1r_Cite = Troncon(Cité_scientifique, Triolo, 642, trace);
    ligne1Troncons.push_back(&ligne1r_Cite);

    // Triolo vers Villeneuve_dAscq (retour)
    // Trace entre les stations Triolo et Villeneuve_dAscq
    trace = { Point2D(207 * coeff_largeur, 424 * coeff_hauteur),
              Point2D((207 + 0.01 * 207) * coeff_largeur, (416 + 0.01 * 416) * coeff_hauteur),
              Point2D(208 * coeff_largeur, 410 * coeff_hauteur) };
    Troncon ligne1r_Triolo = Troncon(Triolo, Villeneuve_dAscq, 919, trace);
    ligne1Troncons.push_back(&ligne1r_Triolo);

    // Villeneuve_dAscq vers Pont_de_Bois (retour)
    // Trace entre les stations Villeneuve_dAscq et Pont_de_Bois
    trace = { Point2D(208 * coeff_largeur, 410 * coeff_hauteur),
              Point2D((208 + 0.01 * 208) * coeff_largeur, (404 + 0.01 * 404) * coeff_hauteur),
              Point2D(209 * coeff_largeur, 400 * coeff_hauteur) };
    Troncon ligne1r_Villeneuve = Troncon(Villeneuve_dAscq, Pont_de_Bois, 639, trace);
    ligne1Troncons.push_back(&ligne1r_Villeneuve);

    // Pont_de_Bois vers Square_Flandres (retour)
    // Trace entre les stations Pont_de_Bois et Square_Flandres
    trace = { Point2D(209 * coeff_largeur, 400 * coeff_hauteur),
              Point2D((218 + 0.01 * 218) * coeff_largeur, (376 + 0.01 * 376) * coeff_hauteur),
              Point2D(227 * coeff_largeur, 383 * coeff_hauteur) };
    Troncon ligne1r_PontDeBois = Troncon(Pont_de_Bois, Square_Flandres, 1254, trace);
    ligne1Troncons.push_back(&ligne1r_PontDeBois);

    // Square_Flandres vers Mairie_dHellemmes (retour)
    // Trace entre les stations Square_Flandres et Mairie_dHellemmes
    trace = { Point2D(227 * coeff_largeur, 383 * coeff_hauteur),
              Point2D((229 + 0.01 * 229) * coeff_largeur, (376 + 0.01 * 376) * coeff_hauteur),
              Point2D(232 * coeff_largeur, 370 * coeff_hauteur) };
    Troncon ligne1r_SquareFlandres = Troncon(Square_Flandres, Mairie_dHellemmes, 209, trace);
    ligne1Troncons.push_back(&ligne1r_SquareFlandres);

    // Mairie_dHellemmes vers Marbrerie (retour)
    // Trace entre les stations Mairie_dHellemmes et Marbrerie
    trace = { Point2D(232 * coeff_largeur, 370 * coeff_hauteur),
              Point2D((232 + 0.01 * 232) * coeff_largeur, (370 + 0.01 * 370) * coeff_hauteur),
              Point2D(246 * coeff_largeur, 367 * coeff_hauteur) };
    Troncon ligne1r_MairiedHellmes = Troncon(Mairie_dHellemmes, Marbrerie, 567, trace);
    ligne1Troncons.push_back(&ligne1r_MairiedHellmes);

    // Marbrerie vers Fives (retour)
    // Trace entre les stations Marbrerie et Fives
    trace = { Point2D(246 * coeff_largeur, 367 * coeff_hauteur),
              Point2D((263 + 0.01 * 263) * coeff_largeur, (366 + 0.01 * 366) * coeff_hauteur),
              Point2D(279 * coeff_largeur, 366 * coeff_hauteur) };
    Troncon ligne1r_Marbrerie = Troncon(Marbrerie, Fives, 665, trace);
    ligne1Troncons.push_back(&ligne1r_Marbrerie);

    // Fives vers Caulier (retour)
    // Trace entre les stations Fives et Caulier
    trace = { Point2D(279 * coeff_largeur, 366 * coeff_hauteur),
              Point2D((282 + 0.01 * 282) * coeff_largeur, (372 + 0.01 * 372) * coeff_hauteur),
              Point2D(286 * coeff_largeur, 378 * coeff_hauteur) };
    Troncon ligne1r_Fives = Troncon(Fives, Caulier, 582, trace);
    ligne1Troncons.push_back(&ligne1r_Fives);

    // Caulier vers Gare_Lille_Flandres (retour)
    // Trace entre les stations Caulier et Gare_Lille_Flandres
    trace = { Point2D(286 * coeff_largeur, 378 * coeff_hauteur),
              Point2D((292 + 0.01 * 292) * coeff_largeur, (383 + 0.01 * 383) * coeff_hauteur),
              Point2D(299 * coeff_largeur, 388 * coeff_hauteur) };
    Troncon ligne1r_Caulier = Troncon(Caulier, Gare_Lille_Flandres, 754, trace);
    ligne1Troncons.push_back(&ligne1r_Caulier);

    // Gare_Lille_Flandres vers Rihour (retour)
    // Trace entre les stations Gare_Lille_Flandres et Rihour
    trace = { Point2D(299 * coeff_largeur, 388 * coeff_hauteur),
              Point2D((307 + 0.01 * 307) * coeff_largeur, (393 + 0.01 * 393) * coeff_hauteur),
              Point2D(316 * coeff_largeur, 397 * coeff_hauteur) };
    Troncon ligne1r_Flandres = Troncon(Gare_Lille_Flandres, Rihour, 1240, trace);
    ligne1Troncons.push_back(&ligne1r_Flandres);

    // Rihour vers République_Beaux_Arts (retour)
    // Trace entre les stations Rihour et République_Beaux_Arts
    trace = { Point2D(316 * coeff_largeur, 397 * coeff_hauteur),
              Point2D((321 + 0.01 * 321) * coeff_largeur, (398 + 0.01 * 398) * coeff_hauteur),
              Point2D(326 * coeff_largeur, 399 * coeff_hauteur) };
    Troncon ligne1r_Rhiour = Troncon(Rihour, République_Beaux_Arts, 534, trace);
    ligne1Troncons.push_back(&ligne1r_Rhiour);

    // République_Beaux_Arts vers Gambetta (retour)
    // Trace entre les stations République_Beaux_Arts et Gambetta
    trace = { Point2D(326 * coeff_largeur, 399 * coeff_hauteur),
              Point2D((346 + 0.01 * 346) * coeff_largeur, (401 + 0.01 * 401) * coeff_hauteur),
              Point2D(360 * coeff_largeur, 405 * coeff_hauteur) };
    Troncon ligne1r_Republique = Troncon(République_Beaux_Arts, Gambetta, 928, trace);
    ligne1Troncons.push_back(&ligne1r_Republique);

    // Gambetta vers Wazemmes (retour)
    // Trace entre les stations Gambetta et Wazemmes
    trace = { Point2D(360 * coeff_largeur, 405 * coeff_hauteur),
              Point2D((363 + 0.01 * 363) * coeff_largeur, (413 + 0.01 * 413) * coeff_hauteur),
              Point2D(365 * coeff_largeur, 422 * coeff_hauteur) };
    Troncon ligne1r_Gambetta = Troncon(Gambetta, Wazemmes, 430, trace);
    ligne1Troncons.push_back(&ligne1r_Gambetta);

    // Wazemmes vers Porte_des_Postes (retour)
    // Trace entre les stations Wazemmes et Porte_des_Postes
    trace = { Point2D(365 * coeff_largeur, 422 * coeff_hauteur),
              Point2D((379 + 0.01 * 379) * coeff_largeur, (420 + 0.01 * 420) * coeff_hauteur),
              Point2D(386 * coeff_largeur, 429 * coeff_hauteur) };
    Troncon ligne1r_Wazemes = Troncon(Wazemmes, Porte_des_Postes, 505, trace);
    ligne1Troncons.push_back(&ligne1r_Wazemes);

    // Porte_des_Postes vers CHU_Centre_Oscar_Lambret (retour)
    // Trace entre les stations Porte_des_Postes et CHU_Centre_Oscar_Lambret
    trace = { Point2D(386 * coeff_largeur, 429 * coeff_hauteur),
              Point2D((389 + 0.01 * 389) * coeff_largeur, (438 + 0.01 * 438) * coeff_hauteur),
              Point2D(389 * coeff_largeur, 446 * coeff_hauteur) };
    Troncon ligne1r_PorteDesPostes = Troncon(Porte_des_Postes, CHU_Centre_Oscar_Lambret, 1301, trace);
    ligne1Troncons.push_back(&ligne1r_PorteDesPostes);

    // CHU_Centre_Oscar_Lambret vers CHU_Eurasanté (retour)
    // Trace entre les stations CHU_Centre_Oscar_Lambret et CHU_Eurasanté
    trace = { Point2D(389 * coeff_largeur, 446 * coeff_hauteur),
              Point2D((384 + 0.01 * 384) * coeff_largeur, (455 + 0.01 * 45) * coeff_hauteur),
              Point2D(383 * coeff_largeur, 466 * coeff_hauteur) };
    Troncon ligne1r_CHU_Centre = Troncon(CHU_Centre_Oscar_Lambret, CHU_Eurasanté, 542, trace);
    ligne1Troncons.push_back(&ligne1r_CHU_Centre);





    //Lien entre troncons

    ligne1_CHU_Eurasante.setTronconSuivant(ligne1_CHU_Centre_Oscar_Lambert);
    ligne1_CHU_Centre_Oscar_Lambert.setTronconSuivant(ligne1_Porte_des_Postes);
    ligne1_Porte_des_Postes.setTronconSuivant(ligne1_Wazemes);
    ligne1_Wazemes.setTronconSuivant(ligne1_Gambetta);
    ligne1_Gambetta.setTronconSuivant(ligne1_Republique);
    ligne1_Republique.setTronconSuivant(ligne1_Rhiour);
    ligne1_Rhiour.setTronconSuivant(ligne1_Flandres);
    ligne1_Flandres.setTronconSuivant(ligne1_Caulier);
    ligne1_Caulier.setTronconSuivant(ligne1_Fives);
    ligne1_Fives.setTronconSuivant(ligne1_Marbrerie);
    ligne1_Marbrerie.setTronconSuivant(ligne1_MairiedHellmes);
    ligne1_MairiedHellmes.setTronconSuivant(ligne1_SquareFlandres);
    ligne1_SquareFlandres.setTronconSuivant(ligne1_PontDeBois);
    ligne1_PontDeBois.setTronconSuivant(ligne1_Villeneuve);
    ligne1_Villeneuve.setTronconSuivant(ligne1_Triolo);
    ligne1_Triolo.setTronconSuivant(ligne1_Cite);
    ligne1_Cite.setTronconSuivant(ligne1r_Quatre_Cantons);

    //Ligne n°1 : Le retour 
    ligne1r_Quatre_Cantons.setTronconSuivant(ligne1r_Cite);
    ligne1r_Cite.setTronconSuivant(ligne1r_Triolo);
    ligne1r_Triolo.setTronconSuivant(ligne1r_Villeneuve);
    ligne1r_Villeneuve.setTronconSuivant(ligne1r_PontDeBois);
    ligne1r_PontDeBois.setTronconSuivant(ligne1r_SquareFlandres);
    ligne1r_SquareFlandres.setTronconSuivant(ligne1r_MairiedHellmes);
    ligne1r_MairiedHellmes.setTronconSuivant(ligne1r_Marbrerie);
    ligne1r_Marbrerie.setTronconSuivant(ligne1r_Fives);
    ligne1r_Fives.setTronconSuivant(ligne1r_Caulier);
    ligne1r_Caulier.setTronconSuivant(ligne1r_Flandres);
    ligne1r_Flandres.setTronconSuivant(ligne1r_Rhiour);
    ligne1r_Rhiour.setTronconSuivant(ligne1r_Republique);
    ligne1r_Republique.setTronconSuivant(ligne1r_Gambetta);
    ligne1r_Gambetta.setTronconSuivant(ligne1r_Wazemes);
    ligne1r_Wazemes.setTronconSuivant(ligne1r_PorteDesPostes);
    ligne1r_PorteDesPostes.setTronconSuivant(ligne1r_CHU_Centre);
    ligne1r_CHU_Centre.setTronconSuivant(ligne1_CHU_Eurasante);
    Ligne Ligne1 = Ligne();

    Ligne1.setListeStation(ligne1Stations);
    Ligne1.setListeTroncon(ligne1Troncons);
    vector<Ligne*> listeLignes = { &Ligne1 };


    vector<Rame*>listeRames = {};

    /*
    Station Saint_Philibert("Saint Philibert", false, 0, false, true, 54 * coeff_largeur, 320 * coeff_hauteur);
    Station Bourg("Bourg", false, 0, false, false, 76 * coeff_largeur, 340 * coeff_hauteur);
    Station Maison_des_enfants("Maison des enfants", false, 0, false, false, 94 * coeff_largeur, 337 * coeff_hauteur);
    Station Mitterie("Mitterie", false, 0, false, false, 121 * coeff_largeur, 335 * coeff_hauteur);
    Station Pont_Supérieur("Pont Supérieur", false, 0, false, false, 132 * coeff_largeur, 343 * coeff_hauteur);
    Station Lomme("Lomme", false, 0, false, false, 141 * coeff_largeur, 357 * coeff_hauteur);
    Station Canteleu("Canteleu", false, 0, false, false, 154 * coeff_largeur, 367 * coeff_hauteur);
    Station Bois_Blancs("Bois Blancs", false, 0, false, false, 166 * coeff_largeur, 376 * coeff_hauteur);
    Station Port_de_Lille("Port de Lille", false, 0, false, false, 176 * coeff_largeur, 390 * coeff_hauteur);
    Station Cormontaigne("Cormontaigne", false, 0, false, false, 185 * coeff_largeur, 401 * coeff_hauteur);
    Station Montebello("Montebello", false, 0, false, false, 196 * coeff_largeur, 414 * coeff_hauteur);
    Station Porte_Arras("Porte d'Arras", false, 0, false, false, 229 * coeff_largeur, 428 * coeff_hauteur);
    Station Porte_Douai("Porte de Douai", false, 0, false, false, 248 * coeff_largeur, 427 * coeff_hauteur);
    Station Porte_Valenciennes("Porte de Valenciennes", false, 0, false, false, 264 * coeff_largeur, 418 * coeff_hauteur);
    Station Lille_Grand_Palais("Lille Grand Palais", false, 0, false, false, 256 * coeff_largeur, 390 * coeff_hauteur);
    Station Mairie_Lille("Maire de Lille", false, 0, false, false, 247 * coeff_largeur, 379 * coeff_hauteur);
    Station Gare_Lille_Europe("Gare Lille-Europe", false, 0, false, false, 256 * coeff_largeur, 358 * coeff_hauteur);
    Station Saint_Maurice("Saint-Maurice Pellevoisin", false, 0, false, false, 282 * coeff_largeur, 349 * coeff_hauteur);
    Station Mons_Sarts("Mons Sarts", false, 0, false, false, 313 * coeff_largeur, 349 * coeff_hauteur);
    Station Mairie_Mons("Mairie de Mons", false, 0, false, false, 322 * coeff_largeur, 350 * coeff_hauteur);
    Station Fort_Mons("Fort de Mons", false, 0, false, false, 346 * coeff_largeur, 349 * coeff_hauteur);
    Station Les_Prés("Les Prés Edgard-Pisani", false, 0, false, false, 358 * coeff_largeur, 325 * coeff_hauteur);
    Station Jean_Jaurès("Jean-Jaurès", false, 0, false, false, 375 * coeff_largeur, 298 * coeff_hauteur);
    Station Wasquehal_Pavé("Wasquehal-Pavé de Lille", false, 0, false, false, 366 * coeff_largeur, 282 * coeff_hauteur);
    Station Wasquehal_Hotel("Wasquehal-Hotel de Ville", false, 0, false, false, 367 * coeff_largeur, 265 * coeff_hauteur);
    Station Croix_Centre("Croix-Centre", false, 0, false, false, 398 * coeff_largeur, 248 * coeff_hauteur);
    Station Mairie_Croix("Mairie de Croix", false, 0, false, false, 416 * coeff_largeur, 233 * coeff_hauteur);
    Station Epeule("Epeule-Montesquieu", false, 0, false, false, 431 * coeff_largeur, 219 * coeff_hauteur);
    Station Roubaix_Charles_Gaulle("Roubaix", false, 0, false, false, 445 * coeff_largeur, 209 * coeff_hauteur);
    Station Eurotéléport("Eurotéléport", false, 0, false, false, 463 * coeff_largeur, 196 * coeff_hauteur);
    Station Roubaix_Grand_Place("Roubaix-Grand-Place", false, 0, false, false, 450 * coeff_largeur, 193 * coeff_hauteur);
    Station Gare_Jean_Lebas("Gare Jean-Lebas Roubaix", false, 0, false, false, 432 * coeff_largeur, 182 * coeff_hauteur);
    Station Alsace("Alsace", false, 0, false, false, 427 * coeff_largeur, 168 * coeff_hauteur);
    Station Mercure("Mercure", false, 0, false, false, 425 * coeff_largeur, 152 * coeff_hauteur);
    Station Carliers("Carliers", false, 0, false, false, 424 * coeff_largeur, 136 * coeff_hauteur);
    Station Gare_Tourcoing("Gare de Tourcoing", false, 0, false, false, 429 * coeff_largeur, 117 * coeff_hauteur);
    Station Tourcoing_Centre("Tourcoing-Centre", false, 0, false, false, 423 * coeff_largeur, 102 * coeff_hauteur);
    Station Colbert("Colbert", false, 0, false, false, 418 * coeff_largeur, 91 * coeff_hauteur);
    Station Phalempins("Phalempins", false, 0, false, false, 419 * coeff_largeur, 69 * coeff_hauteur);
    Station Pont_de_Neuville("Pont de Neuville", false, 0, false, false, 446 * coeff_largeur, 54 * coeff_hauteur);
    Station Bourgogne("Bourgogne", false, 0, false, false, 464 * coeff_largeur, 45 * coeff_hauteur);
    Station CH_Dron("CH Dron", false, 0, true, false, 465 * coeff_largeur, 29 * coeff_hauteur);


    vector<Station*> ligne2Stations = { &Saint_Philibert, &Bourg, &Maison_des_enfants, &Mitterie, &Pont_Supérieur, &Lomme, &Canteleu, &Bois_Blancs, &Port_de_Lille, &Cormontaigne, &Montebello, &Porte_des_Postes, &Porte_Arras, &Porte_Douai, &Porte_Valenciennes, &Lille_Grand_Palais, &Mairie_Lille, &Gare_Lille_Flandres, &Gare_Lille_Europe, &Saint_Maurice, &Mons_Sarts, &Mairie_Mons, &Fort_Mons, &Les_Prés, &Jean_Jaurès, &Wasquehal_Pavé, &Wasquehal_Hotel, &Croix_Centre, &Mairie_Croix, &Epeule, &Roubaix_Charles_Gaulle, &Eurotéléport, &Roubaix_Grand_Place, &Gare_Jean_Lebas, &Alsace, &Mercure, &Carliers, &Gare_Tourcoing, &Tourcoing_Centre, &Colbert, &Phalempins, &Pont_de_Neuville, &Bourgogne, &CH_Dron };

    vector<Troncon*>ligne2Troncons = {};

    vector<Point2D> trace1 = { Point2D(54 * coeff_largeur,320 * coeff_hauteur),Point2D(62 * coeff_largeur,330 * coeff_hauteur),Point2D(76 * coeff_largeur,340 * coeff_hauteur) };
    Troncon ligne2_Saint_Philibert(Saint_Philibert, Bourg, 1075, trace1);
    ligne2Troncons.push_back(&ligne2_Saint_Philibert);

    trace1 = { Point2D(76 * coeff_largeur, 340 * coeff_hauteur), Point2D(108 * coeff_largeur, 335 * coeff_hauteur), Point2D(121 * coeff_largeur, 335 * coeff_hauteur) };
    Troncon ligne2_Bourg(Bourg, Maison_des_enfants, 667, trace1);
    ligne2Troncons.push_back(&ligne2_Bourg);

    trace1 = { Point2D(76 * coeff_largeur, 340 * coeff_hauteur), Point2D(108 * coeff_largeur, 335 * coeff_hauteur), Point2D(121 * coeff_largeur, 335 * coeff_hauteur) };
    Troncon ligne2_Maison_des_Enfants(Maison_des_enfants, Mitterie, 931, trace1);
    ligne2Troncons.push_back(&ligne2_Maison_des_Enfants);

    trace1 = { Point2D(121 * coeff_largeur, 335 * coeff_hauteur), Point2D(128 * coeff_largeur, 338 * coeff_hauteur), Point2D(132 * coeff_largeur, 343 * coeff_hauteur) };
    Troncon ligne2_Mitterie(Mitterie, Pont_Supérieur, 3497, trace1);
    ligne2Troncons.push_back(&ligne2_Mitterie);

    trace1 = { Point2D(132 * coeff_largeur, 343 * coeff_hauteur), Point2D(137 * coeff_largeur, 349 * coeff_hauteur), Point2D(141 * coeff_largeur, 357 * coeff_hauteur) };
    Troncon ligne2_Pont_Supérieur(Pont_Supérieur, Lomme, 4163, trace1);
    ligne2Troncons.push_back(&ligne2_Pont_Supérieur);

    trace1 = { Point2D(141 * coeff_largeur, 357 * coeff_hauteur), Point2D(146 * coeff_largeur, 360 * coeff_hauteur), Point2D(154 * coeff_largeur, 367 * coeff_hauteur) };
    Troncon ligne2_Lomme(Lomme, Canteleu, 589, trace1);
    ligne2Troncons.push_back(&ligne2_Lomme);

    trace1 = { Point2D(154 * coeff_largeur, 367 * coeff_hauteur), Point2D(160 * coeff_largeur, 370 * coeff_hauteur), Point2D(166 * coeff_largeur, 376 * coeff_hauteur) };
    Troncon ligne2_Canteleu(Canteleu, Bois_Blancs, 521, trace1);
    ligne2Troncons.push_back(&ligne2_Canteleu);

    trace1 = { Point2D(166 * coeff_largeur, 376 * coeff_hauteur), Point2D(171 * coeff_largeur, 382 * coeff_hauteur), Point2D(176 * coeff_largeur, 390 * coeff_hauteur) };
    Troncon ligne2_Bois_Blancs(Bois_Blancs, Port_de_Lille, 564, trace1);
    ligne2Troncons.push_back(&ligne2_Bois_Blancs);

    trace1 = { Point2D(176 * coeff_largeur, 390 * coeff_hauteur), Point2D(181 * coeff_largeur, 396 * coeff_hauteur), Point2D(185 * coeff_largeur, 401 * coeff_hauteur) };
    Troncon ligne2_Port_de_Lille(Port_de_Lille, Cormontaigne, 631, trace1);
    ligne2Troncons.push_back(&ligne2_Port_de_Lille);

    trace1 = { Point2D(185 * coeff_largeur, 401 * coeff_hauteur), Point2D(189 * coeff_largeur, 407 * coeff_hauteur), Point2D(196 * coeff_largeur, 414 * coeff_hauteur) };
    Troncon ligne2_Cormontaigne(Cormontaigne, Montebello, 557, trace1);
    ligne2Troncons.push_back(&ligne2_Cormontaigne);

    trace1 = { Point2D(196 * coeff_largeur, 414 * coeff_hauteur), Point2D(200 * coeff_largeur, 419 * coeff_hauteur), Point2D(207 * coeff_largeur, 424 * coeff_hauteur) };
    Troncon ligne2_Montebello(Montebello, Porte_des_Postes, 507, trace1);
    ligne2Troncons.push_back(&ligne2_Montebello);

    trace1 = { Point2D(207 * coeff_largeur, 424 * coeff_hauteur), Point2D(218 * coeff_largeur, 426 * coeff_hauteur), Point2D(229 * coeff_largeur, 428 * coeff_hauteur) };
    Troncon ligne2_Porte_des_Postes(Porte_des_Postes, Porte_Arras, 868, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_des_Postes);

    trace1 = { Point2D(229 * coeff_largeur, 428 * coeff_hauteur), Point2D(239 * coeff_largeur, 428 * coeff_hauteur), Point2D(248 * coeff_largeur, 427 * coeff_hauteur) };
    Troncon ligne2_Porte_Arras(Porte_Arras, Porte_Douai, 708, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Arras);

    trace1 = { Point2D(248 * coeff_largeur, 427 * coeff_hauteur), Point2D(258 * coeff_largeur, 424 * coeff_hauteur), Point2D(264 * coeff_largeur, 418 * coeff_hauteur) };
    Troncon ligne2_Porte_Douai(Porte_Douai, Porte_Valenciennes, 550, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Douai);

    trace1 = { Point2D(264 * coeff_largeur, 418 * coeff_hauteur), Point2D(262 * coeff_largeur, 402 * coeff_hauteur), Point2D(256 * coeff_largeur, 390 * coeff_hauteur) };
    Troncon ligne2_Porte_Valenciennes(Porte_Valenciennes, Lille_Grand_Palais, 1081, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Valenciennes);

    trace1 = { Point2D(256 * coeff_largeur, 390 * coeff_hauteur), Point2D(251 * coeff_largeur, 384 * coeff_hauteur), Point2D(247 * coeff_largeur, 379 * coeff_hauteur) };
    Troncon ligne2_Lille_Grand_Palais(Lille_Grand_Palais, Mairie_Lille, 455, trace1);
    ligne2Troncons.push_back(&ligne2_Lille_Grand_Palais);

    trace1 = { Point2D(247 * coeff_largeur, 379 * coeff_hauteur), Point2D(246 * coeff_largeur, 374 * coeff_hauteur), Point2D(246 * coeff_largeur, 368 * coeff_hauteur) };
    Troncon ligne2_Marie_Lille(Mairie_Lille, Gare_Lille_Flandres, 523, trace1);
    ligne2Troncons.push_back(&ligne2_Marie_Lille);

    trace1 = { Point2D(246 * coeff_largeur, 368 * coeff_hauteur), Point2D(250 * coeff_largeur, 362 * coeff_hauteur), Point2D(256 * coeff_largeur, 358 * coeff_hauteur) };
    Troncon ligne2_Gare_Lille_Flandres(Gare_Lille_Flandres, Gare_Lille_Europe, 466, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Lille_Flandres);

    trace1 = { Point2D(256 * coeff_largeur, 358 * coeff_hauteur), Point2D(269 * coeff_largeur, 353 * coeff_hauteur), Point2D(282 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2_Garde_Lille_Europe(Gare_Lille_Europe, Saint_Maurice, 917, trace1);
    ligne2Troncons.push_back(&ligne2_Garde_Lille_Europe);

    trace1 = { Point2D(282 * coeff_largeur, 349 * coeff_hauteur), Point2D(292 * coeff_largeur, 349 * coeff_hauteur), Point2D(313 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2_Saint_Maurice(Saint_Maurice, Mons_Sarts, 729, trace1);
    ligne2Troncons.push_back(&ligne2_Saint_Maurice);

    trace1 = { Point2D(313 * coeff_largeur, 349 * coeff_hauteur), Point2D(313 * coeff_largeur, 349 * coeff_hauteur), Point2D(322 * coeff_largeur, 350 * coeff_hauteur) };
    Troncon ligne2_Mons_Sarts(Mons_Sarts, Mairie_Mons, 764, trace1);
    ligne2Troncons.push_back(&ligne2_Mons_Sarts);

    trace1 = { Point2D(322 * coeff_largeur, 350 * coeff_hauteur), Point2D(334 * coeff_largeur, 351 * coeff_hauteur), Point2D(346 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2_Marie_Mons(Mairie_Mons, Fort_Mons, 687, trace1);
    ligne2Troncons.push_back(&ligne2_Marie_Mons);
    trace1 = { Point2D(346 * coeff_largeur, 349 * coeff_hauteur), Point2D(353 * coeff_largeur, 337 * coeff_hauteur), Point2D(358 * coeff_largeur, 325 * coeff_hauteur) };
    Troncon ligne2_Fort_Mons(Fort_Mons, Les_Prés, 1018, trace1);
    ligne2Troncons.push_back(&ligne2_Fort_Mons);

    trace1 = { Point2D(358 * coeff_largeur, 325 * coeff_hauteur), Point2D(367 * coeff_largeur, 310 * coeff_hauteur), Point2D(375 * coeff_largeur, 298 * coeff_hauteur) };
    Troncon ligne2_Les_Prés(Les_Prés, Jean_Jaurès, 1134, trace1);
    ligne2Troncons.push_back(&ligne2_Les_Prés);

    trace1 = { Point2D(375 * coeff_largeur, 298 * coeff_hauteur), Point2D(370 * coeff_largeur, 289 * coeff_hauteur), Point2D(366 * coeff_largeur, 282 * coeff_hauteur) };
    Troncon ligne2_Jean_Jaurès(Jean_Jaurès, Wasquehal_Pavé, 706, trace1);
    ligne2Troncons.push_back(&ligne2_Jean_Jaurès);

    trace1 = { Point2D(366 * coeff_largeur, 282 * coeff_hauteur), Point2D(365 * coeff_largeur, 274 * coeff_hauteur), Point2D(367 * coeff_largeur, 265 * coeff_hauteur) };
    Troncon ligne2_Wasquehal_Pavé(Wasquehal_Pavé, Wasquehal_Hotel, 612, trace1);
    ligne2Troncons.push_back(&ligne2_Wasquehal_Pavé);

    trace1 = { Point2D(367 * coeff_largeur, 265 * coeff_hauteur), Point2D(384 * coeff_largeur, 256 * coeff_hauteur), Point2D(398 * coeff_largeur, 248 * coeff_hauteur) };
    Troncon ligne2_Wasquehal_Hotel(Wasquehal_Hotel, Croix_Centre, 1243, trace1);
    ligne2Troncons.push_back(&ligne2_Wasquehal_Hotel);

    trace1 = { Point2D(398 * coeff_largeur, 248 * coeff_hauteur), Point2D(409 * coeff_largeur, 240 * coeff_hauteur), Point2D(416 * coeff_largeur, 233 * coeff_hauteur) };
    Troncon ligne2_Croix_Centre(Croix_Centre, Mairie_Croix, 850, trace1);
    ligne2Troncons.push_back(&ligne2_Croix_Centre);

    trace1 = { Point2D(416 * coeff_largeur, 233 * coeff_hauteur), Point2D(424 * coeff_largeur, 225 * coeff_hauteur), Point2D(431 * coeff_largeur, 219 * coeff_hauteur) };
    Troncon ligne2_Mairie_Croix(Mairie_Croix, Epeule, 768, trace1);
    ligne2Troncons.push_back(&ligne2_Mairie_Croix);

    trace1 = { Point2D(431 * coeff_largeur, 219 * coeff_hauteur), Point2D(438 * coeff_largeur, 214 * coeff_hauteur), Point2D(445 * coeff_largeur, 209 * coeff_hauteur) };
    Troncon ligne2_Epeule(Epeule, Roubaix_Charles_Gaulle, 531, trace1);
    ligne2Troncons.push_back(&ligne2_Epeule);

    trace1 = { Point2D(445 * coeff_largeur, 209 * coeff_hauteur), Point2D(461 * coeff_largeur, 205 * coeff_hauteur), Point2D(463 * coeff_largeur, 196 * coeff_hauteur) };
    Troncon ligne2_Roubaix_Charles_Gaulle(Roubaix_Charles_Gaulle, Eurotéléport, 823, trace1);
    ligne2Troncons.push_back(&ligne2_Roubaix_Charles_Gaulle);

    trace1 = { Point2D(463 * coeff_largeur, 196 * coeff_hauteur), Point2D(457 * coeff_largeur, 194 * coeff_hauteur), Point2D(450 * coeff_largeur, 193 * coeff_hauteur) };
    Troncon ligne2_Eurotéléport(Eurotéléport, Roubaix_Grand_Place, 382, trace1);
    ligne2Troncons.push_back(&ligne2_Eurotéléport);

    trace1 = { Point2D(450 * coeff_largeur, 193 * coeff_hauteur), Point2D(439 * coeff_largeur, 189 * coeff_hauteur), Point2D(432 * coeff_largeur, 182 * coeff_hauteur) };
    Troncon ligne2_Roubaix_Grand_Place(Roubaix_Grand_Place, Gare_Jean_Lebas, 881, trace1);
    ligne2Troncons.push_back(&ligne2_Roubaix_Grand_Place);

    trace1 = { Point2D(432 * coeff_largeur, 182 * coeff_hauteur), Point2D(429 * coeff_largeur, 182 * coeff_hauteur), Point2D(427 * coeff_largeur, 168 * coeff_hauteur) };
    Troncon ligne2_Gare_Jean_Lebas(Gare_Jean_Lebas, Alsace, 499, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Jean_Lebas);

    trace1 = { Point2D(427 * coeff_largeur, 168 * coeff_hauteur), Point2D(426 * coeff_largeur, 157 * coeff_hauteur), Point2D(425 * coeff_largeur, 152 * coeff_hauteur) };
    Troncon ligne2_Alsace(Alsace, Mercure, 355, trace1);
    ligne2Troncons.push_back(&ligne2_Alsace);

    trace1 = { Point2D(425 * coeff_largeur, 152 * coeff_hauteur), Point2D(425 * coeff_largeur, 142 * coeff_hauteur), Point2D(424 * coeff_largeur, 136 * coeff_hauteur) };
    Troncon ligne2_Mercure(Mercure, Carliers, 746, trace1);
    ligne2Troncons.push_back(&ligne2_Mercure);

    trace1 = { Point2D(424 * coeff_largeur, 136 * coeff_hauteur), Point2D(426 * coeff_largeur, 126 * coeff_hauteur), Point2D(429 * coeff_largeur, 117 * coeff_hauteur) };
    Troncon ligne2_Carliers(Carliers, Gare_Tourcoing, 792, trace1);
    ligne2Troncons.push_back(&ligne2_Carliers);

    trace1 = { Point2D(429 * coeff_largeur, 117 * coeff_hauteur), Point2D(427 * coeff_largeur, 108 * coeff_hauteur), Point2D(423 * coeff_largeur, 102 * coeff_hauteur) };
    Troncon ligne2_Gare_Tourcoing(Gare_Tourcoing, Tourcoing_Centre, 598, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Tourcoing);

    trace1 = { Point2D(423 * coeff_largeur, 102 * coeff_hauteur), Point2D(421 * coeff_largeur, 117 * coeff_hauteur), Point2D(418 * coeff_largeur, 91 * coeff_hauteur) };
    Troncon ligne2_Tourcoing_Centre(Tourcoing_Centre, Colbert, 219, trace1);
    ligne2Troncons.push_back(&ligne2_Tourcoing_Centre);

    trace1 = { Point2D(418 * coeff_largeur, 91 * coeff_hauteur), Point2D(416 * coeff_largeur, 78 * coeff_hauteur), Point2D(419 * coeff_largeur, 69 * coeff_hauteur) };
    Troncon ligne2_Colbert(Colbert, Phalempins, 595, trace1);
    ligne2Troncons.push_back(&ligne2_Colbert);

    trace1 = { Point2D(419 * coeff_largeur, 69 * coeff_hauteur), Point2D(432 * coeff_largeur, 60 * coeff_hauteur), Point2D(446 * coeff_largeur, 54 * coeff_hauteur) };
    Troncon ligne2_Phalempins(Phalempins, Pont_de_Neuville, 1403, trace1);
    ligne2Troncons.push_back(&ligne2_Phalempins);

    trace1 = { Point2D(446 * coeff_largeur, 54 * coeff_hauteur), Point2D(458 * coeff_largeur, 50 * coeff_hauteur), Point2D(464 * coeff_largeur, 45 * coeff_hauteur) };
    Troncon ligne2_Pont_de_Neuville(Pont_de_Neuville, Bourgogne, 658, trace1);
    ligne2Troncons.push_back(&ligne2_Pont_de_Neuville);

    trace1 = { Point2D(464 * coeff_largeur, 45 * coeff_hauteur), Point2D(466 * coeff_largeur, 37 * coeff_hauteur), Point2D(465 * coeff_largeur, 29 * coeff_hauteur) };
    Troncon ligne2_Bourgogne(Bourgogne, CH_Dron, 474, trace1);
    ligne2Troncons.push_back(&ligne2_Bourgogne);




    //Le retouur 

    // Retour de CH_Dron à Bourgogne
    trace1 = { Point2D(465 * coeff_largeur, 29 * coeff_hauteur),
               Point2D(466 * coeff_largeur * 1.01, 37 * coeff_hauteur * 1.01),
               Point2D(464 * coeff_largeur, 45 * coeff_hauteur) };
    Troncon ligne2r_CH_Dron(CH_Dron,Bourgogne, 474, trace1);
    ligne2Troncons.push_back(&ligne2r_CH_Dron);

    // Retour de Bourgogne à Pont_de_Neuville
    trace1 = { Point2D(464 * coeff_largeur, 45 * coeff_hauteur),
               Point2D(458 * coeff_largeur * 1.01, 50 * coeff_hauteur * 1.01),
               Point2D(446 * coeff_largeur, 54 * coeff_hauteur) };
    Troncon ligne2r_Bourgogne(Bourgogne, Pont_de_Neuville, 658, trace1);
    ligne2Troncons.push_back(&ligne2r_Bourgogne);

    // Retour de Pont_de_Neuville à Phalempins
    trace1 = { Point2D(446 * coeff_largeur, 54 * coeff_hauteur),
               Point2D(432 * coeff_largeur * 1.01, 60 * coeff_hauteur * 1.01),
               Point2D(419 * coeff_largeur, 69 * coeff_hauteur) };
    Troncon ligne2r_Pont_de_Neuville(Pont_de_Neuville, Phalempins, 1403, trace1);
    ligne2Troncons.push_back(&ligne2r_Pont_de_Neuville);

    // Retour de Phalempins à Colbert
    trace1 = { Point2D(419 * coeff_largeur, 69 * coeff_hauteur),
               Point2D(416 * coeff_largeur * 1.01, 78 * coeff_hauteur * 1.01),
               Point2D(418 * coeff_largeur, 91 * coeff_hauteur) };
    Troncon ligne2r_Phalempins(Phalempins, Colbert, 595, trace1);
    ligne2Troncons.push_back(&ligne2r_Phalempins);

    // Retour de Colbert à Tourcoing_Centre
    trace1 = { Point2D(418 * coeff_largeur, 91 * coeff_hauteur),
               Point2D(421 * coeff_largeur * 1.01, 117 * coeff_hauteur * 1.01),
               Point2D(423 * coeff_largeur, 102 * coeff_hauteur) };
    Troncon ligne2r_Colbert(Colbert, Tourcoing_Centre, 219, trace1);
    ligne2Troncons.push_back(&ligne2r_Colbert);

    // Retour de Tourcoing_Centre à Gare_Tourcoing
    trace1 = { Point2D(423 * coeff_largeur, 102 * coeff_hauteur),
               Point2D(427 * coeff_largeur * 1.01, 108 * coeff_hauteur * 1.01),
               Point2D(429 * coeff_largeur, 117 * coeff_hauteur) };
    Troncon ligne2r_Tourcoing_Centre(Tourcoing_Centre, Tourcoing_Centre, 598, trace1);
    ligne2Troncons.push_back(&ligne2r_Tourcoing_Centre);

    // Retour de Gare_Tourcoing à Carliers
    trace1 = { Point2D(429 * coeff_largeur, 117 * coeff_hauteur),
               Point2D(426 * coeff_largeur * 1.01, 126 * coeff_hauteur * 1.01),
               Point2D(424 * coeff_largeur, 136 * coeff_hauteur) };
    Troncon ligne2r_Gare_Tourcoing(Gare_Tourcoing, Carliers, 792, trace1);
    ligne2Troncons.push_back(&ligne2r_Gare_Tourcoing);

    // Retour de Carliers à Mercure
    trace1 = { Point2D(424 * coeff_largeur, 136 * coeff_hauteur),
               Point2D(425 * coeff_largeur * 1.01, 142 * coeff_hauteur * 1.01),
               Point2D(425 * coeff_largeur, 152 * coeff_hauteur) };
    Troncon ligne2r_Carliers(Carliers, Mercure, 746, trace1);
    ligne2Troncons.push_back(&ligne2r_Carliers);

    // Retour de Mercure à Alsace
    trace1 = { Point2D(425 * coeff_largeur, 152 * coeff_hauteur),
               Point2D(425 * coeff_largeur * 1.01, 157 * coeff_hauteur * 1.01),
               Point2D(426 * coeff_largeur, 168 * coeff_hauteur) };
    Troncon ligne2r_Mercure(Mercure, Alsace, 355, trace1);
    ligne2Troncons.push_back(&ligne2r_Mercure);

    // Retour de Alsace à Gare_Jean_Lebas
    trace1 = { Point2D(426 * coeff_largeur, 168 * coeff_hauteur),
               Point2D(429 * coeff_largeur * 1.01, 182 * coeff_hauteur * 1.01),
               Point2D(432 * coeff_largeur, 182 * coeff_hauteur) };
    Troncon ligne2r_Alsace(Alsace, Gare_Jean_Lebas, 499, trace1);
    ligne2Troncons.push_back(&ligne2r_Alsace);

    // Retour de Gare_Jean_Lebas à Roubaix_Grand_Place
    trace1 = { Point2D(432 * coeff_largeur, 182 * coeff_hauteur),
               Point2D(439 * coeff_largeur * 1.01, 189 * coeff_hauteur * 1.01),
               Point2D(450 * coeff_largeur, 193 * coeff_hauteur) };
    Troncon ligne2r_Gare_Jean_Lebas(Gare_Jean_Lebas, Roubaix_Grand_Place, 881, trace1);
    ligne2Troncons.push_back(&ligne2r_Gare_Jean_Lebas);

    // Retour de Roubaix_Grand_Place à Eurotéléport
    trace1 = { Point2D(450 * coeff_largeur, 193 * coeff_hauteur),
               Point2D(457 * coeff_largeur * 1.01, 194 * coeff_hauteur * 1.01),
               Point2D(463 * coeff_largeur, 196 * coeff_hauteur) };
    Troncon ligne2r_Roubaix_Grand_Place(Roubaix_Grand_Place, Eurotéléport, 382, trace1);
    ligne2Troncons.push_back(&ligne2r_Roubaix_Grand_Place);

    // Retour de Eurotéléport à Roubaix_Charles_Gaulle
    trace1 = { Point2D(463 * coeff_largeur, 196 * coeff_hauteur),
               Point2D(461 * coeff_largeur * 1.01, 205 * coeff_hauteur * 1.01),
               Point2D(445 * coeff_largeur, 209 * coeff_hauteur) };
    Troncon ligne2r_Eurotéléport(Eurotéléport, Roubaix_Charles_Gaulle, 823, trace1);
    ligne2Troncons.push_back(&ligne2r_Eurotéléport);

    // Retour de Roubaix_Charles_Gaulle à Epeule
    trace1 = { Point2D(445 * coeff_largeur, 209 * coeff_hauteur),
               Point2D(438 * coeff_largeur * 1.01, 214 * coeff_hauteur * 1.01),
               Point2D(431 * coeff_largeur, 219 * coeff_hauteur) };
    Troncon ligne2r_Roubaix_Charles_Gaulle(Roubaix_Charles_Gaulle, Epeule, 531, trace1);
    ligne2Troncons.push_back(&ligne2r_Roubaix_Charles_Gaulle);

    // Retour de Epeule à Mairie_Croix
    trace1 = { Point2D(431 * coeff_largeur, 219 * coeff_hauteur),
               Point2D(424 * coeff_largeur * 1.01, 225 * coeff_hauteur * 1.01),
               Point2D(416 * coeff_largeur, 233 * coeff_hauteur) };
    Troncon ligne2r_Epeule(Epeule, Mairie_Croix, 768, trace1);
    ligne2Troncons.push_back(&ligne2r_Epeule);

    // Retour de Mairie_Croix à Croix_Centre
    trace1 = { Point2D(416 * coeff_largeur, 233 * coeff_hauteur),
               Point2D(409 * coeff_largeur * 1.01, 240 * coeff_hauteur * 1.01),
               Point2D(398 * coeff_largeur, 248 * coeff_hauteur) };
    Troncon ligne2r_Mairie_Croix(Mairie_Croix, Croix_Centre, 850, trace1);
    ligne2Troncons.push_back(&ligne2r_Mairie_Croix);

    // Retour de Croix_Centre à Wasquehal_Hotel
    trace1 = { Point2D(398 * coeff_largeur, 248 * coeff_hauteur),
               Point2D(384 * coeff_largeur * 1.01, 256 * coeff_hauteur * 1.01),
               Point2D(367 * coeff_largeur, 265 * coeff_hauteur) };
    Troncon ligne2r_Croix_Centre(Wasquehal_Hotel, Croix_Centre, 1243, trace1);
    ligne2Troncons.push_back(&ligne2r_Croix_Centre);

    // Retour de Wasquehal_Hotel à Wasquehal_Pavé
    trace1 = { Point2D(367 * coeff_largeur, 265 * coeff_hauteur),
               Point2D(365 * coeff_largeur * 1.01, 274 * coeff_hauteur * 1.01),
               Point2D(366 * coeff_largeur, 282 * coeff_hauteur) };
    Troncon ligne2r_Wasquehal_Hotel(Wasquehal_Hotel, Wasquehal_Pavé, 612, trace1);
    ligne2Troncons.push_back(&ligne2r_Wasquehal_Hotel);
    
    // Retour de Wasquehal_Pavé à Jean_Jaurès
    trace1 = { Point2D(367 * coeff_largeur, 265 * coeff_hauteur),
               Point2D(370 * coeff_largeur * 1.01, 289 * coeff_hauteur * 1.01),
               Point2D(375 * coeff_largeur, 298 * coeff_hauteur) };
    Troncon ligne2r_Wasquehal_Pavé(Wasquehal_Pavé, Jean_Jaurès, 706, trace1);
    ligne2Troncons.push_back(&ligne2r_Wasquehal_Pavé);

    // Retour de Jean_Jaurès à Les_Prés
    trace1 = { Point2D(375 * coeff_largeur, 298 * coeff_hauteur),
               Point2D(370 * coeff_largeur * 1.01, 289 * coeff_hauteur * 1.01),
               Point2D(358 * coeff_largeur, 325 * coeff_hauteur) };
    Troncon ligne2r_Jean_Jaurès(Jean_Jaurès, Les_Prés, 1134, trace1);
    ligne2Troncons.push_back(&ligne2r_Jean_Jaurès);

    //Retour de Les Prés à Fort_Mons
    trace1 = { Point2D(358 * coeff_largeur, 325 * coeff_hauteur),
               Point2D(353 * coeff_largeur * 1.01, 337 * coeff_hauteur * 1.01),
               Point2D(346 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2r_Les_Prés(Les_Prés, Fort_Mons, 1018, trace1);
    ligne2Troncons.push_back(&ligne2r_Les_Prés);
   
    //Retour de Fort_Mons à Mairie_Mons
    trace1 = { Point2D(346 * coeff_largeur, 349 * coeff_hauteur),
               Point2D(334 * coeff_largeur * 1.01, 351 * coeff_hauteur * 1.01),
               Point2D(322 * coeff_largeur, 350 * coeff_hauteur) };
    Troncon ligne2r_Fort_Mons(Fort_Mons, Mairie_Mons, 687, trace1);
    ligne2Troncons.push_back(&ligne2r_Fort_Mons);

    //Retour de  Mairie_Mons à Mons_Sarts
    trace1 = { Point2D(322 * coeff_largeur, 350 * coeff_hauteur),
               Point2D(313 * coeff_largeur * 1.01, 349 * coeff_hauteur * 1.01),
               Point2D(303 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2r_Mairie_Mons(Mairie_Mons, Mons_Sarts, 764, trace1);
    ligne2Troncons.push_back(&ligne2r_Mairie_Mons);

    //Retour de Mons_Sarts à Saint_Maurice
    trace1 = { Point2D(303 * coeff_largeur, 349 * coeff_hauteur),
               Point2D(292 * coeff_largeur * 1.01, 349 * coeff_hauteur * 1.01),
               Point2D(282 * coeff_largeur, 349 * coeff_hauteur) };
    Troncon ligne2r_Mons_Sarts(Mons_Sarts, Saint_Maurice, 729, trace1);
    ligne2Troncons.push_back(&ligne2r_Mons_Sarts);

    //Retour de Saint_Maurice à Gare_Lille_Europe 
    trace1 = { Point2D(282 * coeff_largeur, 349 * coeff_hauteur),
               Point2D(269 * coeff_largeur * 1.01, 353 * coeff_hauteur * 1.01),
               Point2D(256 * coeff_largeur, 358 * coeff_hauteur) };
    Troncon ligne2r_Saint_Maurice(Saint_Maurice, Gare_Lille_Europe, 917, trace1);
    ligne2Troncons.push_back(&ligne2r_Saint_Maurice);

    //Retour de Gare_Lille_Europe à Gare_Lille_Flandres
    trace1 = { Point2D(256 * coeff_largeur, 358 * coeff_hauteur),
               Point2D(250 * coeff_largeur * 1.01, 362 * coeff_hauteur * 1.01),
               Point2D(246 * coeff_largeur, 368 * coeff_hauteur) };
    Troncon ligne2r_Gare_Lille_Europe(Gare_Lille_Europe, Gare_Lille_Flandres, 466, trace1);
    ligne2Troncons.push_back(&ligne2r_Gare_Lille_Europe);

    //Retour de Gare_Lille_Flandres à Mairie_Lille
    trace1 = { Point2D(246 * coeff_largeur, 368 * coeff_hauteur),
               Point2D(246 * coeff_largeur * 1.01, 374 * coeff_hauteur * 1.01),
               Point2D(247 * coeff_largeur, 379 * coeff_hauteur) };
    Troncon ligne2r_Gare_Lille_Flandres(Gare_Lille_Flandres, Mairie_Lille, 523, trace1);
    ligne2Troncons.push_back(&ligne2r_Gare_Lille_Flandres);

    //Retour de Mairie_Lille à Lille_Grand_Palais
    trace1 = { Point2D(247 * coeff_largeur, 379 * coeff_hauteur),
               Point2D(251 * coeff_largeur * 1.01, 384 * coeff_hauteur * 1.01),
               Point2D(256 * coeff_largeur, 390 * coeff_hauteur) };
    Troncon ligne2r_Mairie_Lille(Mairie_Lille, Lille_Grand_Palais, 455, trace1);
    ligne2Troncons.push_back(&ligne2r_Mairie_Lille);

    //Retour de Lille_Grand_Palais à Porte_Valenciennes
    trace1 = { Point2D(256 * coeff_largeur, 390 * coeff_hauteur),
               Point2D(262 * coeff_largeur * 1.01, 402 * coeff_hauteur * 1.01),
               Point2D(264 * coeff_largeur, 418 * coeff_hauteur) };
    Troncon ligne2r_Lille_Grand_Palais(Lille_Grand_Palais, Porte_Valenciennes, 1081, trace1);
    ligne2Troncons.push_back(&ligne2r_Lille_Grand_Palais);

    //Retour de Porte_Valenciennes à Porte_Douai
    trace1 = { Point2D(264 * coeff_largeur, 418 * coeff_hauteur),
               Point2D(258 * coeff_largeur * 1.01, 424 * coeff_hauteur * 1.01),
               Point2D(248 * coeff_largeur, 427 * coeff_hauteur) };
    Troncon ligne2r_Porte_Valenciennes(Porte_Valenciennes, Porte_Douai, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Porte_Valenciennes);

    //Retour de Porte_Douai à Porte_Arras
    trace1 = { Point2D(248 * coeff_largeur, 427 * coeff_hauteur),
               Point2D(239 * coeff_largeur * 1.01, 428 * coeff_hauteur * 1.01),
               Point2D(229 * coeff_largeur, 428 * coeff_hauteur) };
    Troncon ligne2r_Porte_Douai(Porte_Douai, Porte_Arras, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Porte_Douai);

    //Retour de Porte_Arras à Porte_des_Postes
    trace1 = { Point2D(229 * coeff_largeur, 428 * coeff_hauteur),
               Point2D(218 * coeff_largeur * 1.01, 426 * coeff_hauteur * 1.01),
               Point2D(207 * coeff_largeur, 424 * coeff_hauteur) };
    Troncon ligne2r_Porte_Arras(Porte_Arras, Porte_des_Postes, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Porte_Arras);

    //Retour de Porte_des_Postes à Montebello
    trace1 = { Point2D(207 * coeff_largeur, 424 * coeff_hauteur),
               Point2D(200 * coeff_largeur * 1.01, 419 * coeff_hauteur * 1.01),
               Point2D(196 * coeff_largeur, 414 * coeff_hauteur) };
    Troncon ligne2r_Porte_des_Postes(Porte_des_Postes, Montebello, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Porte_des_Postes);

    //Retour de Montebello à Cormontaigne 
    trace1 = { Point2D(196 * coeff_largeur, 414 * coeff_hauteur),
               Point2D(189 * coeff_largeur * 1.01, 407 * coeff_hauteur * 1.01),
               Point2D(185 * coeff_largeur, 401 * coeff_hauteur) };
    Troncon ligne2r_Montebello(Montebello, Cormontaigne, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Montebello);

    //Retour de Cormontaigne à Port_de_Lille
    trace1 = { Point2D(185 * coeff_largeur, 401 * coeff_hauteur),
               Point2D(181 * coeff_largeur * 1.01, 396 * coeff_hauteur * 1.01),
               Point2D(176 * coeff_largeur, 390 * coeff_hauteur) };
    Troncon ligne2r_Cormontaigne(Cormontaigne, Port_de_Lille, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Cormontaigne);

    //Retour de Port_de_Lille à Bois_Blancs
    trace1 = { Point2D(176 * coeff_largeur, 390 * coeff_hauteur),
               Point2D(171 * coeff_largeur * 1.01, 382 * coeff_hauteur * 1.01),
               Point2D(166 * coeff_largeur, 376 * coeff_hauteur) };
    Troncon ligne2r_Port_de_Lille(Port_de_Lille, Bois_Blancs, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Port_de_Lille);

    //Retour de Bois_Blancs à Canteleu
    trace1 = { Point2D(166 * coeff_largeur, 376 * coeff_hauteur),
               Point2D(160 * coeff_largeur * 1.01, 370 * coeff_hauteur * 1.01),
               Point2D(154 * coeff_largeur, 367 * coeff_hauteur) };
    Troncon ligne2r_Bois_Blancs(Bois_Blancs, Canteleu, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Bois_Blancs);

    //Retour de Canteleu à Lomme
    trace1 = { Point2D(154 * coeff_largeur, 367 * coeff_hauteur),
               Point2D(146 * coeff_largeur * 1.01, 360 * coeff_hauteur * 1.01),
               Point2D(141 * coeff_largeur, 357 * coeff_hauteur) };
    Troncon ligne2r_Canteleu(Canteleu, Lomme, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Canteleu);

    //Retour de Lomme à Pont_Supérieur
    trace1 = { Point2D(141 * coeff_largeur, 357 * coeff_hauteur),
               Point2D(137 * coeff_largeur * 1.01, 349 * coeff_hauteur * 1.01),
               Point2D(132 * coeff_largeur, 343 * coeff_hauteur) };
    Troncon ligne2r_Lomme(Lomme, Pont_Supérieur, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Lomme);

    //Retour de Pont_Supérieur à Mitterie
    trace1 = { Point2D(132 * coeff_largeur, 343 * coeff_hauteur),
               Point2D(128 * coeff_largeur * 1.01, 338 * coeff_hauteur * 1.01),
               Point2D(121 * coeff_largeur, 335 * coeff_hauteur) };
    Troncon ligne2r_Pont_Supérieur(Pont_Supérieur, Mitterie, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Pont_Supérieur);

    //Retour de Mitterie à Maison_des_enfants
    trace1 = { Point2D(121 * coeff_largeur, 335 * coeff_hauteur),
               Point2D(108 * coeff_largeur * 1.01, 335 * coeff_hauteur * 1.01),
               Point2D(94 * coeff_largeur, 337 * coeff_hauteur) };
    Troncon ligne2r_Mitterie(Mitterie, Maison_des_enfants, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Mitterie);

    //Retour de Maison_des_enfants à Bourg
    trace1 = { Point2D(94 * coeff_largeur, 337 * coeff_hauteur),
               Point2D(86 * coeff_largeur * 1.01, 339 * coeff_hauteur * 1.01),
               Point2D(76 * coeff_largeur, 340 * coeff_hauteur) };
    Troncon ligne2r_Maison_des_enfants(Maison_des_enfants, Bourg, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Maison_des_enfants);

    //Retour de Bourg à Saint_Philibert
    trace1 = { Point2D(76 * coeff_largeur, 340 * coeff_hauteur),
               Point2D(62 * coeff_largeur * 1.01, 330 * coeff_hauteur * 1.01),
               Point2D(54 * coeff_largeur, 320 * coeff_hauteur) };
    Troncon ligne2r_Bourg(Bourg, Saint_Philibert, 550, trace1);
    ligne2Troncons.push_back(&ligne2r_Bourg);



    ligne2_Saint_Philibert.setTronconSuivant(ligne2_Bourg);
    ligne2_Bourg.setTronconSuivant(ligne2_Maison_des_Enfants);
    ligne2_Maison_des_Enfants.setTronconSuivant(ligne2_Mitterie);
    ligne2_Mitterie.setTronconSuivant(ligne2_Pont_Supérieur);
    ligne2_Pont_Supérieur.setTronconSuivant(ligne2_Lomme);
    ligne2_Lomme.setTronconSuivant(ligne2_Canteleu);
    ligne2_Canteleu.setTronconSuivant(ligne2_Bois_Blancs);
    ligne2_Bois_Blancs.setTronconSuivant(ligne2_Port_de_Lille);
    ligne2_Port_de_Lille.setTronconSuivant(ligne2_Cormontaigne);
    ligne2_Cormontaigne.setTronconSuivant(ligne2_Montebello);
    ligne2_Montebello.setTronconSuivant(ligne2_Porte_des_Postes);
    ligne2_Porte_des_Postes.setTronconSuivant(ligne2_Porte_Arras);
    ligne2_Porte_Arras.setTronconSuivant(ligne2_Porte_Douai);
    ligne2_Porte_Douai.setTronconSuivant(ligne2_Porte_Valenciennes);
    ligne2_Porte_Valenciennes.setTronconSuivant(ligne2_Lille_Grand_Palais);
    ligne2_Lille_Grand_Palais.setTronconSuivant(ligne2_Marie_Lille);
    ligne2_Marie_Lille.setTronconSuivant(ligne2_Gare_Lille_Flandres);
    ligne2_Gare_Lille_Flandres.setTronconSuivant(ligne2_Garde_Lille_Europe);
    ligne2_Garde_Lille_Europe.setTronconSuivant(ligne2_Saint_Maurice);
    ligne2_Saint_Maurice.setTronconSuivant(ligne2_Mons_Sarts);
    ligne2_Mons_Sarts.setTronconSuivant(ligne2_Marie_Mons);
    ligne2_Marie_Mons.setTronconSuivant(ligne2_Fort_Mons);
    ligne2_Fort_Mons.setTronconSuivant(ligne2_Les_Prés);
    ligne2_Les_Prés.setTronconSuivant(ligne2_Jean_Jaurès);
    ligne2_Jean_Jaurès.setTronconSuivant(ligne2_Wasquehal_Pavé);
    ligne2_Wasquehal_Pavé.setTronconSuivant(ligne2_Wasquehal_Hotel);
    ligne2_Wasquehal_Hotel.setTronconSuivant(ligne2_Croix_Centre);
    ligne2_Croix_Centre.setTronconSuivant(ligne2_Mairie_Croix);
    ligne2_Mairie_Croix.setTronconSuivant(ligne2_Epeule);
    ligne2_Epeule.setTronconSuivant(ligne2_Roubaix_Charles_Gaulle);
    ligne2_Roubaix_Charles_Gaulle.setTronconSuivant(ligne2_Eurotéléport);
    ligne2_Eurotéléport.setTronconSuivant(ligne2_Roubaix_Grand_Place);
    ligne2_Roubaix_Grand_Place.setTronconSuivant(ligne2_Gare_Jean_Lebas);
    ligne2_Gare_Jean_Lebas.setTronconSuivant(ligne2_Alsace);
    ligne2_Alsace.setTronconSuivant(ligne2_Mercure);
    ligne2_Mercure.setTronconSuivant(ligne2_Carliers);
    ligne2_Carliers.setTronconSuivant(ligne2_Gare_Tourcoing);
    ligne2_Gare_Tourcoing.setTronconSuivant(ligne2_Tourcoing_Centre);
    ligne2_Tourcoing_Centre.setTronconSuivant(ligne2_Colbert);
    ligne2_Colbert.setTronconSuivant(ligne2_Phalempins);
    ligne2_Phalempins.setTronconSuivant(ligne2_Pont_de_Neuville);
    ligne2_Pont_de_Neuville.setTronconSuivant(ligne2_Bourgogne);


    //Retour  de CH_Dron à Bourgogne
    ligne2r_CH_Dron.setTronconSuivant(ligne2r_Bourgogne);

    // Retour de Bourgogne à Pont_de_Neuville
    ligne2r_Bourgogne.setTronconSuivant(ligne2r_Pont_de_Neuville);

    // Retour de Pont_de_Neuville à Phalempins
    ligne2r_Pont_de_Neuville.setTronconSuivant(ligne2r_Phalempins);

    // Retour de Phalempins à Colbert
    ligne2r_Phalempins.setTronconSuivant(ligne2r_Colbert);

    // Retour de Colbert à Tourcoing_Centre
    ligne2r_Colbert.setTronconSuivant(ligne2r_Tourcoing_Centre);

    // Retour de Tourcoing_Centre à Gare_Tourcoing
    ligne2r_Tourcoing_Centre.setTronconSuivant(ligne2r_Gare_Tourcoing);

    // Retour de Gare_Tourcoing à Carliers
    ligne2r_Gare_Tourcoing.setTronconSuivant(ligne2r_Carliers);

    // Retour de Carliers à Mercure
    ligne2r_Carliers.setTronconSuivant(ligne2r_Mercure);

    // Retour de Mercure à Alsace
    ligne2r_Mercure.setTronconSuivant(ligne2r_Alsace);

    // Retour de Alsace à Gare_Jean_Lebas
    ligne2r_Alsace.setTronconSuivant(ligne2r_Gare_Jean_Lebas);

    // Retour de Gare_Jean_Lebas à Roubaix_Grand_Place
    ligne2r_Gare_Jean_Lebas.setTronconSuivant(ligne2r_Roubaix_Grand_Place);

    // Retour de Roubaix_Grand_Place à Eurotéléport
    ligne2r_Roubaix_Grand_Place.setTronconSuivant(ligne2r_Eurotéléport);

    // Retour de Eurotéléport à Roubaix_Charles_Gaulle
    ligne2r_Eurotéléport.setTronconSuivant(ligne2r_Roubaix_Charles_Gaulle);

    // Retour de Roubaix_Charles_Gaulle à Epeule
    ligne2r_Roubaix_Charles_Gaulle.setTronconSuivant(ligne2r_Epeule);

    // Retour de Epeule à Mairie de Croix
    ligne2r_Epeule.setTronconSuivant(ligne2r_Mairie_Croix);

    // Retour de Mairie de Croix à Croix Centre
    ligne2r_Mairie_Croix.setTronconSuivant(ligne2r_Croix_Centre);

    // Retour de Croix Centre à Wasquehal Hotel 
    ligne2r_Croix_Centre.setTronconSuivant(ligne2r_Wasquehal_Hotel);

    // Retour de Wasquehal Hotel à Wasquehal Pavé
    ligne2r_Wasquehal_Hotel.setTronconSuivant(ligne2r_Wasquehal_Pavé);

    //Retour de Wasquehal Pavé à Jean Jaures
    ligne2r_Wasquehal_Pavé.setTronconSuivant(ligne2r_Jean_Jaurès);



      //Faaire les changements
      // 
      // 
      // 
    //Retour de Wasquehal Pavé à Jean Jaures
    ligne2r_Jean_Jaurès.setTronconSuivant(ligne2r_Les_Prés);

    ligne2r_Fort_Mons.setTronconSuivant(ligne2r_Mairie_Mons);

    ligne2r_Mairie_Mons.setTronconSuivant(ligne2r_Mons_Sarts);

    ligne2r_Mons_Sarts.setTronconSuivant(ligne2r_Saint_Maurice);

    ligne2r_Saint_Maurice.setTronconSuivant(ligne2r_Gare_Lille_Europe);

    ligne2r_Gare_Lille_Europe.setTronconSuivant(ligne2r_Gare_Lille_Flandres);

    ligne2r_Gare_Lille_Flandres.setTronconSuivant(ligne2r_Mairie_Lille);

    ligne2r_Mairie_Lille.setTronconSuivant(ligne2r_Lille_Grand_Palais);

    ligne2r_Lille_Grand_Palais.setTronconSuivant(ligne2r_Porte_Valenciennes);

    ligne2r_Porte_Valenciennes.setTronconSuivant(ligne2r_Porte_Douai);

    ligne2r_Porte_Douai.setTronconSuivant(ligne2r_Porte_Arras);

    ligne2r_Porte_Arras.setTronconSuivant(ligne2r_Porte_des_Postes);

    ligne2r_Porte_des_Postes.setTronconSuivant(ligne2r_Montebello);



    // Retour de Montebello à Cormontaigne
    ligne2r_Montebello.setTronconSuivant(ligne2r_Cormontaigne);

    // Retour de Cormontaigne à Port de Lille
    ligne2r_Cormontaigne.setTronconSuivant(ligne2r_Port_de_Lille);

    // Retour de Port de Lille à Bois Blancs
    ligne2r_Port_de_Lille.setTronconSuivant(ligne2r_Bois_Blancs);

    // Retour de Bois Blancs à Canteleu
    ligne2r_Bois_Blancs.setTronconSuivant(ligne2r_Canteleu);

    // Retour de Canteleu à Lomme
    ligne2r_Canteleu.setTronconSuivant(ligne2r_Lomme);

    // Retour de Lomme à Pont Supérieur
    ligne2r_Lomme.setTronconSuivant(ligne2r_Pont_Supérieur);

    // Retour de Pont Supérieur à Mitterie
    ligne2r_Pont_Supérieur.setTronconSuivant(ligne2r_Mitterie);

    // Retour de Mitterie à Maison des Enfants
    ligne2r_Mitterie.setTronconSuivant(ligne2r_Maison_des_enfants);

    // Retour de Maison des Enfants à Bourg
    ligne2r_Maison_des_enfants.setTronconSuivant(ligne2r_Bourg);



    

    Ligne Ligne2 = Ligne();

    Ligne2.setListeStation(ligne2Stations);
    Ligne2.setListeTroncon(ligne2Troncons);
    vector<Ligne*> listeLignes1 = { &Ligne2 };
    */

    vector<Rame*>listeRames1 = {};


    Superviseur reseau = Superviseur();
    reseau.listeLignes = listeLignes;
    reseau.listeRames = listeRames;
    reseau.listeStation = ligne1Stations;
    /*
    Superviseur reseau1 = Superviseur();
    reseau1.listeStation = ligne2Stations;
    reseau1.listeLignes = listeLignes1;
    reseau1.listeRames = listeRames1;
    */




    //-----------------------Thread------------------------------------------------//

    thread threadConteur(iteratorRames, std::ref(reseau));

    //---------------------Création des fenetres----------------------------------//
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "VAL", sf::Style::Fullscreen);



    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed) {
                window.close();
                Open = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape){
                window.close();
                Open = false;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                AddRame(reseau);
            }

        }
        window.clear();
        affichage(window,reseau);
        //affichage(window, reseau1);
        window.display();
    }

    
    threadConteur.join();

    

	return 0;
}
