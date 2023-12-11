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
                rame->setPositionTroncon(rame->getPositionTroncon() + 1);

            }
        }
        this_thread::sleep_for(chrono::milliseconds(10));
    }
}



void affichage(sf::RenderWindow &window,Superviseur& reseau) {
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
        
        sf::Font font;
        if (!font.loadFromFile(path_font + "arial.ttf"))
        {
            cout << "La police n'a pas pu être chargé";
        }

        sf::Text text;


        text.setFont(font);
        text.setString(station->getNom()+"\n"+to_string(station->getPAX_quai())+" passagers en attente");
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
        sf::CircleShape shape(5.f);
        shape.setFillColor(sf::Color(100, 250, 50));
        shape.setOrigin(5.f, 5.f);
        shape.setPosition(newPosX, newPosY);
        window.draw(shape);
    }

    this_thread::sleep_for(chrono::milliseconds(75));
}



int main()
{
	
    //Création ligne
   // gestion présence des personnes 
   //ligne n°1 : 
   //Création des stations : 
    Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, 0, false, true,1434,1045);
    Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, 0, false, false,1468,935);
    Station Triolo("Triolo", false, 0, false, false,1449,840);
    Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, 0, false, false,1330,796);
    Station Pont_de_Bois("Pont de Bois", false, 0, false, false,1300,705);
    Station Square_Flandres("Square Flandres", false, 0, false, false,1115,671);
    Station Mairie_dHellemmes("Mairie d'Hellemmes", false, 0, false, false,1085,666);
    Station Marbrerie("Marbrerie", false, 0, false, false,1005,638);
    Station Fives("Fives", false, 0, false, false,921,583);
    Station Caulier("Caulier", false, 0, false, false,867,516);
    Station Gare_Lille_Flandres("Gare Lille-Flandres", false, 0, false, false,770,486);
    Station Rihour("Rihour", false, 0, false, false,584,507);
    Station République_Beaux_Arts("République - Beaux-Arts", false, 0, false, false,561, 584);
    Station Gambetta("Gambetta", false, 0, false, false,453,673);
    Station Wazemmes("Wazemmes", false, 0, false, false,452,738);
    Station Porte_des_Postes("Porte des Postes", false, 0, false, false,446,814);
    Station CHU_Centre_Oscar_Lambret("CHU - Centre Oscar-Lambret", false, 0, false, false,285,926);
    Station CHU_Eurasanté("CHU - Eurasanté", false, 0, true, false,302,1005);




    vector<Station*>ligne1Stations = { &CHU_Eurasanté, &CHU_Centre_Oscar_Lambret, &Porte_des_Postes, &Wazemmes, &Gambetta, &République_Beaux_Arts, &Rihour, &Gare_Lille_Flandres, &Caulier, &Fives, &Marbrerie, &Mairie_dHellemmes, &Square_Flandres, &Pont_de_Bois, &Villeneuve_dAscq, &Triolo, &Cité_scientifique, &Quatre_Cantons };
    vector<Troncon*>ligne1Troncons = {};
    std::vector<Point2D> trace = { Point2D(302,1005),Point2D(298,957),Point2D(285,926)};
    Troncon ligne1_CHU_Eurasante(CHU_Eurasanté,CHU_Centre_Oscar_Lambret, 542,trace);
    ligne1Troncons.push_back(&ligne1_CHU_Eurasante);

    trace = { Point2D(285,926),Point2D(364,865),Point2D(446,814) };
    Troncon ligne1_CHU_Centre_Oscar_Lambert(CHU_Centre_Oscar_Lambret,Porte_des_Postes, 1301, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Centre_Oscar_Lambert);

    trace = {Point2D(446,814),Point2D(449,776),Point2D(452,738)};
    Troncon ligne1_Porte_des_Postes(Porte_des_Postes,Wazemmes, 505, trace);
    ligne1Troncons.push_back(&ligne1_Porte_des_Postes);

    trace = {Point2D(452,738),Point2D(452,702),Point2D(453,673)};
    Troncon ligne1_Wazemes(Wazemmes,Gambetta, 430, trace);
    ligne1Troncons.push_back(&ligne1_Wazemes);

    trace = {Point2D(453,673),Point2D(506,626),Point2D(561,584)};
    Troncon ligne1_Gambetta(Gambetta,République_Beaux_Arts, 928, trace);
    ligne1Troncons.push_back(&ligne1_Gambetta);

    trace = {Point2D(561,584), Point2D(571,538),Point2D(584,507)};
    Troncon ligne1_Republique(République_Beaux_Arts,Rihour, 534, trace);
    ligne1Troncons.push_back(&ligne1_Republique);

    trace = {Point2D(584,507),Point2D(667,496), Point2D(770,486) };
    Troncon ligne1_Rhiour(Rihour,Gare_Lille_Flandres ,1240, trace);
    ligne1Troncons.push_back(&ligne1_Rhiour);

    trace = { Point2D(770,486), Point2D(845,484),Point2D(867,516) };
    Troncon ligne1_Flandres(Gare_Lille_Flandres,Caulier, 754, trace);
    ligne1Troncons.push_back(&ligne1_Flandres);

    trace = {Point2D(867,516), Point2D(888,556), Point2D(921,583)};
    Troncon ligne1_Caulier(Caulier,Fives, 582, trace);
    ligne1Troncons.push_back(&ligne1_Caulier);

    trace = {Point2D(921,583),Point2D(963,609),Point2D(1005,638) };
    Troncon ligne1_Fives(Fives,Marbrerie , 665, trace);
    ligne1Troncons.push_back(&ligne1_Fives);

    
    Ligne Ligne1 = Ligne();

    Ligne1.setListeStation(ligne1Stations);
    Ligne1.setListeTroncon(ligne1Troncons);
    vector<Ligne*> listeLignes = { &Ligne1 };

    Rame rame1 = Rame();
    rame1.setNumero(1);
    rame1.setPAX(10);
    rame1.setPositionTroncon(50);
    rame1.setTronconActuel(&ligne1_CHU_Eurasante);

    ligne1_CHU_Eurasante.addRameSurTroncon(rame1);

    vector<Rame*>listeRames = { &rame1 };





    Superviseur reseau = Superviseur();

    reseau.listeLignes = listeLignes;
    reseau.listeRames = listeRames;
    reseau.listeStation = ligne1Stations;



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

        }
        window.clear();
        affichage(window,reseau);
        window.display();
    }

    
    threadConteur.join();

    

	return 0;
}
