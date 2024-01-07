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
	
    //Création ligne
   // gestion présence des personnes 
   //ligne n°1 : 
   //Création des stations : 
    Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, 0, true, true,1434,1045);
    Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, 0, false, false,1468,935);
    Station Triolo("Triolo", false, 0, false, false,1449,840);
    Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, 0, false, false,1330,796);
    Station Pont_de_Bois("Pont de Bois", false, 0, false, false,1300,705);
    Station Square_Flandres("Square Flandres", false, 0, false, false,1145,691);
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
    //Création de la liste des tronçons de la ligne n°1
    vector<Troncon*>ligne1Troncons = {};
    vector<Point2D> trace = { Point2D(183, 457),
                          Point2D(181 , 450 ),
                          Point2D(178 , 444) };
    Troncon ligne1_CHU_Eurasante(CHU_Eurasanté, CHU_Centre_Oscar_Lambret, 542, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Eurasante);

    trace = { Point2D(285,926),Point2D(364,865),Point2D(446,814) };
    Troncon ligne1_CHU_Centre_Oscar_Lambert(CHU_Centre_Oscar_Lambret,Porte_des_Postes, 1301, trace);
    ligne1Troncons.push_back(&ligne1_CHU_Centre_Oscar_Lambert);

    trace = {Point2D(441,814),Point2D(444,776),Point2D(447,738)};
    Troncon ligne1_Porte_des_Postes(Porte_des_Postes,Wazemmes, 505, trace);
    ligne1Troncons.push_back(&ligne1_Porte_des_Postes);

    trace = {Point2D(448,738),Point2D(448,702),Point2D(448,673)};
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

    trace = {Point2D(1005,638),Point2D(1056,662),Point2D(1085,666) };
    Troncon ligne1_Marbrerie(Marbrerie,Mairie_dHellemmes, 567, trace);
    ligne1Troncons.push_back(&ligne1_Marbrerie);
    
    trace = { Point2D(1085,666),Point2D(1115,672),Point2D(1145,691) };
    Troncon ligne1_MairiedHellmes(Mairie_dHellemmes,Square_Flandres, 209, trace);
    ligne1Troncons.push_back(&ligne1_MairiedHellmes);

    trace = {Point2D(1145,691), Point2D(1215,679),Point2D(1300,705) };
    Troncon ligne1_SquareFlandres(Square_Flandres,Pont_de_Bois , 1254, trace);
    ligne1Troncons.push_back(&ligne1_SquareFlandres);

    trace = {Point2D(1300,705), Point2D(1321,751), Point2D(1330,796) };
    Troncon ligne1_PontDeBois(Pont_de_Bois,Villeneuve_dAscq, 639, trace);
    ligne1Troncons.push_back(&ligne1_PontDeBois);

    trace = {Point2D(1330,796), Point2D(1405,794), Point2D(1449,840)};
    Troncon ligne1_Villeneuve(Villeneuve_dAscq,Triolo, 919, trace);
    ligne1Troncons.push_back(&ligne1_Villeneuve);

    trace = {Point2D(1445,840),Point2D(1460,889), Point2D(1468,935) };
    Troncon ligne1_Triolo(Triolo,Cité_scientifique, 642, trace);
    ligne1Troncons.push_back(&ligne1_Triolo);

    trace = {Point2D(1465,935),Point2D(1439,986),Point2D(1434,1045) };
    Troncon ligne1_Cite(Cité_scientifique,Quatre_Cantons, 781, trace);
    ligne1Troncons.push_back(&ligne1_Cite);


    //

   
  
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
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::A) {
                AddRame(reseau);
            }

        }
        window.clear();
        affichage(window,reseau);
        window.display();
    }

    
    threadConteur.join();

    

	return 0;
}
