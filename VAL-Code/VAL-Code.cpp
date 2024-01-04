// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "Rame.hpp"
#include "Station.h"
#include "troncon.hpp"
#include "superviseur.hpp"
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <random>

using namespace std;

bool Open = true;
int numRames;




float distancePoints(vector<Point2D> lstPoints) {
    float distTotale = 0;
    for (int i = 0; i < lstPoints.size()-1; i++) {
        float diffX = lstPoints[i + 1].posX - lstPoints[i].posX;
        float diffY = lstPoints[i + 1].posY - lstPoints[i].posY;
        distTotale += (float)sqrt(pow(diffX, 2) + pow(diffY, 2));
    }
    return distTotale;
}


void iteratorRames(Superviseur& reseau) {
    while (Open) {
        for (int iRames = 0; iRames < reseau.listeRames.size(); iRames++) {
            Rame* rame = reseau.listeRames[iRames];
            Troncon* troncon = rame->getTronconActuel();
            if (rame->getPositionTroncon() < troncon->getTailleTroncon()) {
                rame->gesPosition();

            }

            rame->gesVitesse();
          
            
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}



/*void affichage(sf::RenderWindow& window, Superviseur& reseau) {
    // Récupérer la taille de l'écran
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktopMode.width;
    unsigned int screenHeight = desktopMode.height;
    // Facteur d'échelle pour ajuster la taille du métro en fonction de la taille de l'écran
    float scale_factor = std::min(screenWidth / 1920.0f, screenHeight / 1080.0f);

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
                lines[i].position = sf::Vector2f(troncon->getTrace()[i].posX, troncon->getTrace()[i].posY);
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
        cercleStation.setPosition(station->getPosX(), station->getPosY());
        cercleStation.setOrigin(10.f, 10.f);
        window.draw(cercleStation);

        //texte
        


        sf::Text text;


        text.setFont(font);
        text.setString(station->getNom()+" ("+to_string(station->getPAX_quai())+" PAX)");
        text.setCharacterSize(16); // exprimée en pixels, pas en points !
        text.setFillColor(sf::Color::White);
        text.setPosition(station->getPosX(), station->getPosY());
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
        text.setString(to_string(rame->getNumero())+"\n"+ to_string((int)round(rame->getVitesse())) + " km/h");
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(16); // exprimée en pixels, pas en points !
        text.setFillColor(sf::Color::White);
        text.setPosition(newPosX,newPosY);
        window.draw(text);
    }

    this_thread::sleep_for(chrono::milliseconds(75));
}*/

void affichage(sf::RenderWindow& window, Superviseur& reseau) {
    sf::Font font;
    if (!font.loadFromFile(path_font + "arial.ttf")) {
        cout << "La police n'a pas pu être chargée";
    }

    // Récupérer la taille de l'écran
    sf::VideoMode desktopMode = sf::VideoMode::getDesktopMode();
    unsigned int screenWidth = desktopMode.width;
    unsigned int screenHeight = desktopMode.height;

    // Facteur d'échelle pour ajuster la taille du métro en fonction de la taille de l'écran
    float scale_factor = std::min(screenWidth / 1920.0f, screenHeight / 1080.0f);

    sf::Texture textureMetro;
    if (!textureMetro.loadFromFile(path_img + "metro.png")) {
        cout << "L'image n'a pas pu être chargée";
    }

    sf::Texture textureMetroFrein;
    if (!textureMetroFrein.loadFromFile(path_img + "metroFrein.png")) {
        cout << "L'image n'a pas pu être chargée";
    }

    // Affichage de l'overlay
    sf::Text infos;
    infos.setFont(font);
    infos.setString("Modélisation du VAL\n Commandes:\n Echap: fermer la fenêtre \n A: ajouter une rame");
    infos.setCharacterSize(static_cast<unsigned int>(16 * scale_factor));
    infos.setFillColor(sf::Color::White);
    infos.setPosition(10.f, 10.f);
    window.draw(infos);

    // Affichage des tracés des lignes
    for (int iLignes = 0; iLignes < reseau.listeLignes.size(); iLignes++) {
        Ligne* ligne = reseau.listeLignes[iLignes];
        for (int nTroncon = 0; nTroncon < ligne->getListeTroncon().size(); nTroncon++) {
            Troncon* troncon = ligne->getListeTroncon()[nTroncon];
            sf::VertexArray lines(sf::LinesStrip, troncon->getTrace().size());
            for (size_t i = 0; i < troncon->getTrace().size(); ++i) {
                lines[i].position = sf::Vector2f(troncon->getTrace()[i].posX * scale_factor, troncon->getTrace()[i].posY * scale_factor);
                lines[i].color = sf::Color::Yellow;
            }
            window.draw(lines);
        }
    }

    // Affichage des stations
    for (int iStations = 0; iStations < reseau.listeStation.size(); iStations++) {
        Station* station = reseau.listeStation[iStations];

        // Cercle
        sf::CircleShape cercleStation(10.f * scale_factor);
        cercleStation.setFillColor(sf::Color(250, 50, 50));
        cercleStation.setPosition(station->getPosX() * scale_factor, station->getPosY() * scale_factor);
        cercleStation.setOrigin(10.f * scale_factor, 10.f * scale_factor);
        window.draw(cercleStation);

        // Texte
        sf::Text text;
        text.setFont(font);
        text.setString(station->getNom() + " (" + to_string(station->getPAX_quai()) + " PAX)");
        text.setCharacterSize(static_cast<unsigned int>(16 * scale_factor));
        text.setFillColor(sf::Color::White);
        text.setPosition(station->getPosX() * scale_factor, station->getPosY() * scale_factor);
        window.draw(text);
    }

    // Affichage des rames
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

        // Affichage
        sf::Sprite shape;
        shape.setTexture(textureMetro);
        if (rame->getStatus() == 3) { // Si il freine
            shape.setTexture(textureMetroFrein);
        }
        shape.setScale(0.75 * scale_factor, 0.75 * scale_factor);
        shape.setOrigin(9.75 * scale_factor, 13.5 * scale_factor);
        shape.setPosition(newPosX, newPosY);
        window.draw(shape);

        // Texte
        sf::Text text;
        text.setFont(font);
        text.setString(to_string(rame->getNumero()) + "\n" + to_string((int)round(rame->getVitesse())) + " km/h");
        text.setStyle(sf::Text::Bold);
        text.setCharacterSize(static_cast<unsigned int>(16 * scale_factor));
        text.setFillColor(sf::Color::White);
        text.setPosition(newPosX, newPosY);
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

int nbrpassager() {
    int nombreAléatoire = rand() % 31;
    return nombreAléatoire;
}


int main()
{

    //Création ligne
   // gestion présence des personnes 
   //ligne n°1 : 
   //Création des stations : 
    Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, nbrpassager(), false, true, 383, 466);
    Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, nbrpassager(), false, false, 389, 446);
    Station Triolo("Triolo", false, nbrpassager(), false, false, 386, 429);
    Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, nbrpassager(), false, false, 365, 422);
    Station Pont_de_Bois("Pont de Bois", false, nbrpassager(), false, false, 360, 405);
    Station Square_Flandres("Square Flandres", false, nbrpassager(), false, false, 326, 399);
    Station Mairie_dHellemmes("Mairie d'Hellemmes", false, nbrpassager(), false, false, 316, 397);
    Station Marbrerie("Marbrerie", false, nbrpassager(), false, false, 299, 388);
    Station Fives("Fives", false, nbrpassager(), false, false, 286, 378);
    Station Caulier("Caulier", false, nbrpassager(), false, false, 279, 366);
    Station Gare_Lille_Flandres("Gare Lille-Flandres", false, nbrpassager(), false, false, 246, 367);
    Station Rihour("Rihour", false, nbrpassager(), false, false, 232, 370);
    Station République_Beaux_Arts("République - Beaux-Arts", false, nbrpassager(), false, false, 227, 383);
    Station Gambetta("Gambetta", false, nbrpassager(), false, false, 209, 400);
    Station Wazemmes("Wazemmes", false, nbrpassager(), false, false, 208, 410);
    Station Porte_des_Postes("Porte des Postes", false, nbrpassager(), false, false, 207, 424);
    Station CHU_Centre_Oscar_Lambret("CHU - Centre Oscar-Lambret", false, nbrpassager(), false, false, 178, 444);
    Station CHU_Eurasanté("CHU - Eurasanté", false, nbrpassager(), true, false, 183, 457);



    vector<Station*>ligne1Stations = { &CHU_Eurasanté, &CHU_Centre_Oscar_Lambret, &Porte_des_Postes, &Wazemmes, &Gambetta, &République_Beaux_Arts, &Rihour, &Gare_Lille_Flandres, &Caulier, &Fives, &Marbrerie, &Mairie_dHellemmes, &Square_Flandres, &Pont_de_Bois, &Villeneuve_dAscq, &Triolo, &Cité_scientifique, &Quatre_Cantons };
    vector<Troncon*>ligne1Troncons = {};
    vector<Point2D> trace = { Point2D(183,457),Point2D(181,450),Point2D(178,444) };
    Troncon ligne1_CHU_Eurasante(CHU_Eurasanté, CHU_Centre_Oscar_Lambret, 542, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Eurasante);
    int nombre = ligne1_CHU_Eurasante.getRamesSurLigne().size();


    trace = { Point2D(178,444),Point2D(193,433),Point2D(207,424) };
    Troncon ligne1_CHU_Centre_Oscar_Lambert(CHU_Centre_Oscar_Lambret, Porte_des_Postes, 1301, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Centre_Oscar_Lambert);
    ligne1_CHU_Centre_Oscar_Lambert.gesPasssagers();

    trace = { Point2D(207,424),Point2D(207,416),Point2D(208,410) };
    Troncon ligne1_Porte_des_Postes(Porte_des_Postes, Wazemmes, 505, trace);
    ligne1Troncons.push_back(&ligne1_Porte_des_Postes);

    trace = { Point2D(208,410),Point2D(208,404),Point2D(209,400) };
    Troncon ligne1_Wazemes(Wazemmes, Gambetta, 430, trace);
    ligne1Troncons.push_back(&ligne1_Wazemes);

    trace = { Point2D(209,400),Point2D(218,376),Point2D(227, 383) };
    Troncon ligne1_Gambetta(Gambetta, République_Beaux_Arts, 928, trace);
    ligne1Troncons.push_back(&ligne1_Gambetta);

    trace = { Point2D(227, 383), Point2D(229,376),Point2D(232,370) };
    Troncon ligne1_Republique(République_Beaux_Arts, Rihour, 534, trace);
    ligne1Troncons.push_back(&ligne1_Republique);

    trace = { Point2D(232,370),Point2D(232,370), Point2D(246,367) };
    Troncon ligne1_Rhiour(Rihour, Gare_Lille_Flandres, 1240, trace);
    ligne1Troncons.push_back(&ligne1_Rhiour);

    trace = { Point2D(246,367), Point2D(263,366),Point2D(279,366) };
    Troncon ligne1_Flandres(Gare_Lille_Flandres, Caulier, 754, trace);
    ligne1Troncons.push_back(&ligne1_Flandres);

    trace = { Point2D(279,366), Point2D(282,372), Point2D(286,378) };
    Troncon ligne1_Caulier(Caulier, Fives, 582, trace);
    ligne1Troncons.push_back(&ligne1_Caulier);

    trace = { Point2D(286,378),Point2D(292,383),Point2D(299,388) };
    Troncon ligne1_Fives(Fives, Marbrerie, 665, trace);
    ligne1Troncons.push_back(&ligne1_Fives);

    trace = { Point2D(299,388),Point2D(307,393),Point2D(316,397) };
    Troncon ligne1_Marbrerie(Marbrerie, Mairie_dHellemmes, 567, trace);
    ligne1Troncons.push_back(&ligne1_Marbrerie);

    trace = { Point2D(316,397),Point2D(321,398),Point2D(326,399) };
    Troncon ligne1_MairiedHellmes(Mairie_dHellemmes, Square_Flandres, 209, trace);
    ligne1Troncons.push_back(&ligne1_MairiedHellmes);

    trace = { Point2D(326,399), Point2D(346,401),Point2D(360,405) };
    Troncon ligne1_SquareFlandres(Square_Flandres, Pont_de_Bois, 1254, trace);
    ligne1Troncons.push_back(&ligne1_SquareFlandres);

    trace = { Point2D(360,405), Point2D(363,413), Point2D(365,422) };
    Troncon ligne1_PontDeBois(Pont_de_Bois, Villeneuve_dAscq, 639, trace);
    ligne1Troncons.push_back(&ligne1_PontDeBois);

    trace = { Point2D(365,422), Point2D(379,420), Point2D(386,429) };
    Troncon ligne1_Villeneuve(Villeneuve_dAscq, Triolo, 919, trace);
    ligne1Troncons.push_back(&ligne1_Villeneuve);

    trace = { Point2D(386,429),Point2D(389,438), Point2D(389,446) };
    Troncon ligne1_Triolo(Triolo, Cité_scientifique, 642, trace);
    ligne1Troncons.push_back(&ligne1_Triolo);

    trace = { Point2D(389,446),Point2D(384,455),Point2D(383,466) };
    Troncon ligne1_Cite(Cité_scientifique, Quatre_Cantons, 781, trace);
    ligne1Troncons.push_back(&ligne1_Cite);


    //Retour 

   /*

    // Quatre_Cantons vers Cité_scientifique
    trace = { Point2D(1436,1055), Point2D(1447,990), Point2D(1468,945) };
    Troncon ligne1r_Quatre_Cantons = Troncon(Quatre_Cantons, Cité_scientifique, 781, trace);
    ligne1Troncons.push_back(&ligne1r_Quatre_Cantons);

    // Cité_scientifique vers Triolo
    trace = { Point2D(1472,945), Point2D(1468,899), Point2D(1453,850) };
    Troncon ligne1r_Cite = Troncon(Cité_scientifique, Triolo, 642, trace);
    ligne1Troncons.push_back(&ligne1r_Cite);

    // Triolo vers Villeneuve_dAscq
    trace = { Point2D(1449,850), Point2D(1405,804), Point2D(1330,806) };
    Troncon ligne1r_Triolo = Troncon(Triolo, Villeneuve_dAscq, 919, trace);
    ligne1Troncons.push_back(&ligne1r_Triolo);

    // Villeneuve_dAscq vers Pont_de_Bois
    trace = { Point2D(1340,806), Point2D(1328,743), Point2D(1310,715)};
    Troncon ligne1r_Villeneuve = Troncon(Villeneuve_dAscq, Pont_de_Bois, 639, trace);
    ligne1Troncons.push_back(&ligne1r_Villeneuve);

    // Pont_de_Bois vers Square_Flandres
    trace = { Point2D(1300,715), Point2D(1215,689), Point2D(1145,701) };
    Troncon ligne1r_PontDeBois = Troncon(Pont_de_Bois, Square_Flandres, 1254, trace);
    ligne1Troncons.push_back(&ligne1r_PontDeBois);

    // Square_Flandres vers Mairie_dHellemmes
    trace = { Point2D(1145,701), Point2D(1115,682), Point2D(1085,676) };
    Troncon ligne1r_SquareFlandres = Troncon(Square_Flandres, Mairie_dHellemmes, 209, trace);
    ligne1Troncons.push_back(&ligne1r_SquareFlandres);

    // Mairie_dHellemmes vers Marbrerie
    trace = { Point2D(1085,676), Point2D(1056,672), Point2D(1005,648) };
    Troncon ligne1r_MairiedHellmes = Troncon(Mairie_dHellemmes, Marbrerie, 567, trace);
    ligne1Troncons.push_back(&ligne1r_MairiedHellmes);

    // Marbrerie vers Fives
    trace = { Point2D(1005,648), Point2D(963,619), Point2D(921,593) };
    Troncon ligne1r_Marbrerie = Troncon(Marbrerie, Fives, 665, trace);
    ligne1Troncons.push_back(&ligne1r_Marbrerie);

    // Fives vers Caulier
    trace = { Point2D(921,593), Point2D(888,566), Point2D(867,526) };
    Troncon ligne1r_Fives = Troncon(Fives, Caulier, 582, trace);
    ligne1Troncons.push_back(&ligne1r_Fives);

    // Caulier vers Gare_Lille_Flandres
    trace = { Point2D(867,526), Point2D(845,494), Point2D(770,496) };
    Troncon ligne1r_Caulier = Troncon(Caulier, Gare_Lille_Flandres, 754, trace);
    ligne1Troncons.push_back(&ligne1r_Caulier);

    // Gare_Lille_Flandres vers Rihour
    trace = { Point2D(770,496), Point2D(667,506), Point2D(584,517) };
    Troncon ligne1r_Flandres = Troncon(Gare_Lille_Flandres, Rihour, 1240, trace);
    ligne1Troncons.push_back(&ligne1r_Flandres);

    // Rihour vers République_Beaux_Arts
    trace = { Point2D(584,517), Point2D(571,548), Point2D(561,594) };
    Troncon ligne1r_Rhiour = Troncon(Rihour, République_Beaux_Arts, 534, trace);
    ligne1Troncons.push_back(&ligne1r_Rhiour);

    // République_Beaux_Arts vers Gambetta
    trace = { Point2D(561,594), Point2D(506,636), Point2D(453,683) };
    Troncon ligne1r_Republique = Troncon(République_Beaux_Arts, Gambetta, 928, trace);
    ligne1Troncons.push_back(&ligne1r_Republique);

    // Gambetta vers Wazemmes
    trace = { Point2D(456,683), Point2D(456,712), Point2D(456,748) };
    Troncon ligne1r_Gambetta = Troncon(Gambetta, Wazemmes, 430, trace);
    ligne1Troncons.push_back(&ligne1r_Gambetta);

    // Wazemmes vers Porte_des_Postes
    trace = { Point2D(455,748), Point2D(454,786), Point2D(451,824) };
    Troncon ligne1r_Wazemes = Troncon(Wazemmes, Porte_des_Postes, 505, trace);
    ligne1Troncons.push_back(&ligne1r_Wazemes);

    // Porte_des_Postes vers CHU_Centre_Oscar_Lambret
    trace = { Point2D(446,824), Point2D(364,875), Point2D(285,936) };
    Troncon ligne1r_PorteDesPostes = Troncon(Porte_des_Postes, CHU_Centre_Oscar_Lambret, 1301, trace);
    ligne1Troncons.push_back(&ligne1r_PorteDesPostes);

    // CHU_Centre_Oscar_Lambret vers CHU_Eurasanté
    trace = { Point2D(292,936), Point2D(306,967), Point2D(308,1015) };
    Troncon ligne1r_CHU_Centre = Troncon(CHU_Centre_Oscar_Lambret, CHU_Eurasanté, 542, trace);
    ligne1Troncons.push_back(&ligne1r_CHU_Centre);

    */

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
    /*
    //le retour
    ligne1_Cite.setTronconSuivant(ligne1r_Quatre_Cantons);
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
    */


    Ligne Ligne1 = Ligne();

    Ligne1.setListeStation(ligne1Stations);
    Ligne1.setListeTroncon(ligne1Troncons);
    vector<Ligne*> listeLignes = { &Ligne1 };


    vector<Rame*>listeRames = {};

    //ligne n°2 : 
    Station Saint_Philibert("Saint Philibert", false, nbrpassager(), false, true, 54, 320);
    Station Bourg("Bourg", false, nbrpassager(), false, false, 76, 340);
    Station Maison_des_enfants("Maison des enfants", false, nbrpassager(), false, false, 94, 337);
    Station Mitterie("Mitterie", false, nbrpassager(), false, false, 121, 335);
    Station Pont_Supérieur("Pont Supérieur", false, nbrpassager(), false, false, 132, 343);
    Station Lomme("Lomme", false, nbrpassager(), false, false, 141, 357);
    Station Canteleu("Canteleu", false, nbrpassager(), false, false, 154, 367);
    Station Bois_Blancs("Bois Blancs", false, nbrpassager(), false, false, 166, 376);
    Station Port_de_Lille("Port de Lille", false, nbrpassager(), false, false, 176, 390);
    Station Cormontaigne("Cormontaigne", false, nbrpassager(), false, false, 185, 401);
    Station Montebello("Montebello", false, nbrpassager(), false, false, 196, 414);
    Station Porte_Arras("Porte d'Arras", false, nbrpassager(), false, false, 229, 428);
    Station Porte_Douai("Porte de Douai", false, nbrpassager(), false, false, 248, 427);
    Station Porte_Valenciennes("Porte de Valenciennes", false, nbrpassager(), false, false, 264, 418);
    Station Lille_Grand_Palais("Lille Grand Palais", false, nbrpassager(), false, false, 256, 390);
    Station Marie_Lille("Maire de Lille", false, nbrpassager(), false, false, 247, 379);
    Station Garde_Lille_Europe("Gare Lille-Europe", false, nbrpassager(), false, false, 256, 358);
    Station Saint_Maurice("Saint-Maurice Pellevoisin", false, nbrpassager(), false, false, 282, 349);
    Station Mons_Sarts("Mons Sarts", false, nbrpassager(), false, false, 313, 349);
    Station Marie_Mons("Mairie de Mons", false, nbrpassager(), false, false, 322, 350);
    Station Fort_Mons("Fort de Mons", false, nbrpassager(), false, false, 346, 349);
    Station Les_Prés("Les Prés Edgard-Pisani", false, nbrpassager(), false, false, 358, 325);
    Station Jean_Jaurès("Jean-Jaurès", false, nbrpassager(), false, false, 375, 298);
    Station Wasquehal_Pavé("Wasquehal-Pavé de Lille", false, nbrpassager(), false, false, 366, 282);
    Station Wasquehal_Hotel("Wasquehal-Hotel de Ville", false, nbrpassager(), false, false, 367, 265);
    Station Croix_Centre("Croix-Centre", false, nbrpassager(), false, false, 398, 248);
    Station Mairie_Croix("Mairie de Croix", false, nbrpassager(), false, false, 416, 233);
    Station Epeule("Epeule-Montesquieu", false, nbrpassager(), false, false, 431, 219);
    Station Roubaix_Charles_Gaulle("Roubaix", false, nbrpassager(), false, false, 445, 209);
    Station Eurotéléport("Eurotéléport", false, nbrpassager(), false, false, 463, 196);
    Station Roubaix_Grand_Place("Roubaix-Grand-Place", false, nbrpassager(), false, false, 450, 193);
    Station Gare_Jean_Lebas("Gare Jean-Lebas Roubaix", false, nbrpassager(), false, false, 432, 182);
    Station Alsace("Alsace", false, nbrpassager(), false, false, 427, 168);
    Station Mercure("Mercure", false, nbrpassager(), false, false, 425, 152);
    Station Carliers("Carliers", false, nbrpassager(), false, false, 424, 136);
    Station Gare_Tourcoing("Gare de Tourcoing", false, nbrpassager(), false, false, 429, 117);
    Station Tourcoing_Centre("Tourcoing-Centre", false, nbrpassager(), false, false, 423, 102);
    Station Colbert("Colbert", false, nbrpassager(), false, false, 418, 91);
    Station Phalempins("Phalempins", false, nbrpassager(), false, false, 419, 69);
    Station Pont_de_Neuville("Pont de Neuville", false, nbrpassager(), false, false, 446, 54);
    Station Bourgogne("Bourgogne", false, nbrpassager(), false, false, 464, 45);
    Station CH_Dron("CH Dron", false, nbrpassager(), true, false, 465, 29);

    vector<Station*> ligne2Stations = { &Saint_Philibert, &Bourg, &Maison_des_enfants, &Mitterie, &Pont_Supérieur, &Lomme, &Canteleu, &Bois_Blancs, &Port_de_Lille, &Cormontaigne, &Montebello, &Porte_des_Postes, &Porte_Arras, &Porte_Douai, &Porte_Valenciennes, &Lille_Grand_Palais, &Marie_Lille, &Gare_Lille_Flandres, &Garde_Lille_Europe, &Saint_Maurice, &Mons_Sarts, &Marie_Mons, &Fort_Mons, &Les_Prés, &Jean_Jaurès, &Wasquehal_Pavé, &Wasquehal_Hotel, &Croix_Centre, &Mairie_Croix, &Epeule, &Roubaix_Charles_Gaulle, &Eurotéléport, &Roubaix_Grand_Place, &Gare_Jean_Lebas, &Alsace, &Mercure, &Carliers, &Gare_Tourcoing, &Tourcoing_Centre, &Colbert, &Phalempins, &Pont_de_Neuville, &Bourgogne, &CH_Dron };

    vector<Troncon*>ligne2Troncons = {};

    vector<Point2D> trace1 = { Point2D(54,320),Point2D(62,330),Point2D(76,340) };
    Troncon ligne2_Saint_Philibert(Saint_Philibert, Bourg, 1075, trace1);
    ligne2Troncons.push_back(&ligne2_Saint_Philibert);
    int nombre1 = ligne2_Saint_Philibert.getRamesSurLigne().size();


    trace1 = { Point2D(76,340),Point2D(86,339),Point2D(94,337) };
    Troncon ligne2_Bourg(Bourg, Maison_des_enfants, 667, trace1);
    ligne2Troncons.push_back(&ligne2_Bourg);
    ligne2_Bourg.gesPasssagers();

    trace1 = { Point2D(76,340),Point2D(108,335),Point2D(121,335) };
    Troncon ligne2_Maison_des_Enfants(Maison_des_enfants, Mitterie, 931, trace1);
    ligne2Troncons.push_back(&ligne2_Maison_des_Enfants);
    ligne2_Maison_des_Enfants.gesPasssagers();

    trace1 = { Point2D(121, 335),Point2D(128,338),Point2D(132, 343) };
    Troncon ligne2_Mitterie(Mitterie, Pont_Supérieur, 3497, trace1);
    ligne2Troncons.push_back(&ligne2_Mitterie);
    ligne2_Mitterie.gesPasssagers();

    trace1 = { Point2D(132, 343),Point2D(137,349),Point2D(141, 357) };
    Troncon ligne2_Pont_Supérieur(Pont_Supérieur,Lomme, 4163, trace1);
    ligne2Troncons.push_back(&ligne2_Pont_Supérieur);
    ligne2_Pont_Supérieur.gesPasssagers();

    trace1 = { Point2D(141, 357),Point2D(146,360),Point2D(154, 367) };
    Troncon ligne2_Lomme(Lomme, Canteleu, 589, trace1);
    ligne2Troncons.push_back(&ligne2_Lomme);
    ligne2_Lomme.gesPasssagers();

    trace1 = { Point2D(154, 367),Point2D(160,370),Point2D(166, 376) };
    Troncon ligne2_Canteleu(Canteleu, Bois_Blancs, 521, trace1);
    ligne2Troncons.push_back(&ligne2_Canteleu);
    ligne2_Canteleu.gesPasssagers();

    trace1 = { Point2D(166, 376),Point2D(171,382),Point2D(176, 390) };
    Troncon ligne2_Bois_Blancs(Bois_Blancs, Port_de_Lille, 564, trace1);
    ligne2Troncons.push_back(&ligne2_Bois_Blancs);
    ligne2_Bois_Blancs.gesPasssagers();

    trace1 = { Point2D(176, 390),Point2D(181,396),Point2D(185, 401) };
    Troncon ligne2_Port_de_Lille(Port_de_Lille, Cormontaigne, 631, trace1);
    ligne2Troncons.push_back(&ligne2_Port_de_Lille);
    ligne2_Port_de_Lille.gesPasssagers();

    trace1 = { Point2D(185, 401),Point2D(189,407),Point2D(196, 414) };
    Troncon ligne2_Cormontaigne(Cormontaigne, Montebello, 557, trace1);
    ligne2Troncons.push_back(&ligne2_Cormontaigne);
    ligne2_Cormontaigne.gesPasssagers();

    trace1 = { Point2D(196, 414),Point2D(200,419),Point2D(207, 424) };
    Troncon ligne2_Montebello(Montebello, Porte_des_Postes, 507, trace1);
    ligne2Troncons.push_back(&ligne2_Montebello);
    ligne2_Montebello.gesPasssagers();

    trace1 = { Point2D(207, 424),Point2D(218,426),Point2D(229, 428) };
    Troncon ligne2_Porte_des_Postes(Porte_des_Postes, Porte_Arras, 868, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_des_Postes);
    ligne2_Porte_des_Postes.gesPasssagers();

    trace1 = { Point2D(229, 428),Point2D(239,428),Point2D(248, 427) };
    Troncon ligne2_Porte_Arras(Porte_Arras, Porte_Douai, 708, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Arras);
    ligne2_Porte_Arras.gesPasssagers();

    trace1 = { Point2D(248, 427),Point2D(258,424),Point2D(264, 418) };
    Troncon ligne2_Porte_Douai(Porte_Douai, Porte_Valenciennes, 550, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Douai);
    ligne2_Porte_Douai.gesPasssagers();

    trace1 = { Point2D(264, 418),Point2D(262,402),Point2D(256, 390) };
    Troncon ligne2_Porte_Valenciennes(Porte_Valenciennes, Lille_Grand_Palais, 1081, trace1);
    ligne2Troncons.push_back(&ligne2_Porte_Valenciennes);
    ligne2_Porte_Valenciennes.gesPasssagers();

    trace1 = { Point2D(256, 390),Point2D(251,384),Point2D(247, 379) };
    Troncon ligne2_Lille_Grand_Palais(Lille_Grand_Palais, Marie_Lille, 455, trace1);
    ligne2Troncons.push_back(&ligne2_Lille_Grand_Palais);
    ligne2_Lille_Grand_Palais.gesPasssagers();

    trace1 = { Point2D(247, 379),Point2D(246,374),Point2D(246, 368) };
    Troncon ligne2_Marie_Lille(Marie_Lille, Gare_Lille_Flandres, 523, trace1);
    ligne2Troncons.push_back(&ligne2_Marie_Lille);
    ligne2_Marie_Lille.gesPasssagers();

    trace1 = { Point2D(246, 368),Point2D(250,362),Point2D(256, 358) };
    Troncon ligne2_Gare_Lille_Flandres(Gare_Lille_Flandres, Garde_Lille_Europe, 466, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Lille_Flandres);
    ligne2_Gare_Lille_Flandres.gesPasssagers();

    trace1 = { Point2D(256, 358),Point2D(269,353),Point2D(282, 349) };
    Troncon ligne2_Garde_Lille_Europe(Garde_Lille_Europe, Saint_Maurice, 917, trace1);
    ligne2Troncons.push_back(&ligne2_Garde_Lille_Europe);
    ligne2_Garde_Lille_Europe.gesPasssagers();

    trace1 = { Point2D(282, 349),Point2D(292,349),Point2D(313, 349) };
    Troncon ligne2_Saint_Maurice(Saint_Maurice, Mons_Sarts, 729, trace1);
    ligne2Troncons.push_back(&ligne2_Saint_Maurice);
    ligne2_Saint_Maurice.gesPasssagers();

    trace1 = { Point2D(313, 349),Point2D(313,349),Point2D(322, 350) };
    Troncon ligne2_Mons_Sarts(Mons_Sarts, Marie_Mons, 764, trace1);
    ligne2Troncons.push_back(&ligne2_Mons_Sarts);
    ligne2_Mons_Sarts.gesPasssagers();

    trace1 = { Point2D(322, 350),Point2D(334,351),Point2D(346, 349) };
    Troncon ligne2_Marie_Mons(Marie_Mons, Fort_Mons, 687, trace1);
    ligne2Troncons.push_back(&ligne2_Marie_Mons);
    ligne2_Marie_Mons.gesPasssagers();

    trace1 = { Point2D(346, 349),Point2D(353,337),Point2D(358, 325) };
    Troncon ligne2_Fort_Mons(Fort_Mons, Les_Prés, 1018, trace1);
    ligne2Troncons.push_back(&ligne2_Fort_Mons);
    ligne2_Fort_Mons.gesPasssagers();

    trace1 = { Point2D(358, 325),Point2D(367,310),Point2D(375, 298) };
    Troncon ligne2_Les_Prés(Les_Prés, Jean_Jaurès, 1134, trace1);
    ligne2Troncons.push_back(&ligne2_Les_Prés);
    ligne2_Les_Prés.gesPasssagers();

    trace1 = { Point2D(375, 298),Point2D(370,289),Point2D(366, 282)};
    Troncon ligne2_Jean_Jaurès(Jean_Jaurès, Wasquehal_Pavé, 706, trace1);
    ligne2Troncons.push_back(&ligne2_Jean_Jaurès);
    ligne2_Jean_Jaurès.gesPasssagers();

    trace1 = { Point2D(366, 282),Point2D(365,274),Point2D(367, 265) };
    Troncon ligne2_Wasquehal_Pavé(Wasquehal_Pavé, Wasquehal_Hotel, 612, trace1);
    ligne2Troncons.push_back(&ligne2_Wasquehal_Pavé);
    ligne2_Wasquehal_Pavé.gesPasssagers();

    trace1 = { Point2D(367, 265),Point2D(384,256),Point2D(398, 248) };
    Troncon ligne2_Wasquehal_Hotel(Wasquehal_Hotel, Croix_Centre, 1243, trace1);
    ligne2Troncons.push_back(&ligne2_Wasquehal_Hotel);
    ligne2_Wasquehal_Hotel.gesPasssagers();

    trace1 = { Point2D(398, 248),Point2D(409,240),Point2D(416, 233) };
    Troncon ligne2_Croix_Centre(Croix_Centre, Mairie_Croix, 850, trace1);
    ligne2Troncons.push_back(&ligne2_Croix_Centre);
    ligne2_Croix_Centre.gesPasssagers();

    trace1 = { Point2D(416, 233),Point2D(424,225),Point2D(431, 219) };
    Troncon ligne2_Mairie_Croix(Mairie_Croix, Epeule, 768, trace1);
    ligne2Troncons.push_back(&ligne2_Mairie_Croix);
    ligne2_Mairie_Croix.gesPasssagers();

    trace1 = { Point2D(431, 219),Point2D(438,214),Point2D(445, 209) };
    Troncon ligne2_Epeule(Epeule, Roubaix_Charles_Gaulle, 531, trace1);
    ligne2Troncons.push_back(&ligne2_Epeule);
    ligne2_Epeule.gesPasssagers();

    trace1 = { Point2D(445, 209),Point2D(461,205),Point2D(463, 196) };
    Troncon ligne2_Roubaix_Charles_Gaulle(Roubaix_Charles_Gaulle, Eurotéléport, 823, trace1);
    ligne2Troncons.push_back(&ligne2_Roubaix_Charles_Gaulle);
    ligne2_Roubaix_Charles_Gaulle.gesPasssagers();

    trace1 = { Point2D(463, 196),Point2D(457,194),Point2D(450, 193) };
    Troncon ligne2_Eurotéléport(Eurotéléport, Roubaix_Grand_Place, 382, trace1);
    ligne2Troncons.push_back(&ligne2_Eurotéléport);
    ligne2_Eurotéléport.gesPasssagers();

    trace1 = { Point2D(450, 193),Point2D(439,189),Point2D(432, 182) };
    Troncon ligne2_Roubaix_Grand_Place(Roubaix_Grand_Place, Gare_Jean_Lebas, 881, trace1);
    ligne2Troncons.push_back(&ligne2_Roubaix_Grand_Place);
    ligne2_Roubaix_Grand_Place.gesPasssagers();

    trace1 = { Point2D(432, 182),Point2D(429,182),Point2D(427, 168) };
    Troncon ligne2_Gare_Jean_Lebas(Gare_Jean_Lebas, Alsace, 499, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Jean_Lebas);
    ligne2_Gare_Jean_Lebas.gesPasssagers();

    trace1 = { Point2D(427, 168),Point2D(426,157),Point2D(425, 152) };
    Troncon ligne2_Alsace(Alsace, Mercure, 355, trace1);
    ligne2Troncons.push_back(&ligne2_Alsace);
    ligne2_Alsace.gesPasssagers();

    trace1 = { Point2D(425, 152),Point2D(425,142),Point2D(424, 136) };
    Troncon ligne2_Mercure(Mercure, Carliers, 746, trace1);
    ligne2Troncons.push_back(&ligne2_Mercure);
    ligne2_Mercure.gesPasssagers();

    trace1 = { Point2D(424, 136),Point2D(426,126),Point2D(429, 117) };
    Troncon ligne2_Carliers(Carliers, Gare_Tourcoing, 792, trace1);
    ligne2Troncons.push_back(&ligne2_Carliers);
    ligne2_Carliers.gesPasssagers();

    trace1 = { Point2D(429, 117),Point2D(427,108),Point2D(423, 102) };
    Troncon ligne2_Gare_Tourcoing(Gare_Tourcoing, Tourcoing_Centre, 598, trace1);
    ligne2Troncons.push_back(&ligne2_Gare_Tourcoing);
    ligne2_Gare_Tourcoing.gesPasssagers();

    trace1 = { Point2D(423, 102),Point2D(421,117),Point2D(418, 91) };
    Troncon ligne2_Tourcoing_Centre(Tourcoing_Centre, Colbert, 219, trace1);
    ligne2Troncons.push_back(&ligne2_Tourcoing_Centre);
    ligne2_Tourcoing_Centre.gesPasssagers();

    trace1 = { Point2D(418, 91),Point2D(416,78),Point2D(419, 69) };
    Troncon ligne2_Colbert(Colbert, Phalempins, 595, trace1);
    ligne2Troncons.push_back(&ligne2_Colbert);
    ligne2_Colbert.gesPasssagers();

    trace1 = { Point2D(419, 69),Point2D(432,60),Point2D(446, 54) };
    Troncon ligne2_Phalempins(Phalempins, Pont_de_Neuville, 1403, trace1);
    ligne2Troncons.push_back(&ligne2_Phalempins);
    ligne2_Phalempins.gesPasssagers();

    trace1 = { Point2D(446, 54),Point2D(458,50),Point2D(464, 45) };
    Troncon ligne2_Pont_de_Neuville(Pont_de_Neuville, Bourgogne, 658, trace1);
    ligne2Troncons.push_back(&ligne2_Pont_de_Neuville);
    ligne2_Pont_de_Neuville.gesPasssagers();

    trace1 = { Point2D(464, 45),Point2D(466,37),Point2D(465, 29) };
    Troncon ligne2_Bourgogne(Bourgogne, CH_Dron, 474, trace1);
    ligne2Troncons.push_back(&ligne2_Bourgogne);
    ligne2_Bourgogne.gesPasssagers();

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
    

    Ligne Ligne2 = Ligne();

    Ligne2.setListeStation(ligne2Stations);
    Ligne2.setListeTroncon(ligne2Troncons);
    vector<Ligne*> listeLignes1 = { &Ligne2 };


    vector<Rame*>listeRames1 = {};




    

    Superviseur reseau = Superviseur();

    Superviseur reseau1 = Superviseur();

    reseau.listeLignes = listeLignes;
    reseau.listeRames = listeRames;
    reseau.listeStation = ligne1Stations;

    reseau1.listeStation = ligne2Stations;
    reseau1.listeLignes = listeLignes1;
    reseau1.listeRames = listeRames1;



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
        affichage(window, reseau);
        affichage(window,reseau1);
        window.display();
    }

    
    threadConteur.join();

    

	return 0;
}
