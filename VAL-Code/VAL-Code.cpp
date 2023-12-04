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



float distancePoints(vector<Point2D> lstPoints) {
    float distTotale = 0;
    for (int i = 0; i < lstPoints.size()-1; i++) {
        float diffX = lstPoints[i + 1].posX - lstPoints[i].posX;
        float diffY = lstPoints[i + 1].posY - lstPoints[i].posY;
        distTotale += (float)sqrt(pow(diffX, 2) + pow(diffY, 2));
    }
    return distTotale;
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
            }
            window.draw(lines);
        }
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
        while(i<troncon->getTrace().size()-1 && !depasse)
        {
            vector<Point2D> morceau = { troncon->getTrace()[i],troncon->getTrace()[i + 1] };
                if (distParcouruePx + distancePoints(morceau) < positionPx) {
                    compteurPointsPasses++;
                    distParcouruePx += distancePoints(morceau);
                }
                else {
                    depasse = true;
                    distanceTroncon = distancePoints(morceau);
                }
                i++;
        }

        float distanceSurTroncon = rame->getPositionTroncon() - distParcouruePx;
        float pourcentageSurTroncon = distanceTroncon / distanceSurTroncon;
        float distanceTronconX = (float)troncon->getTrace()[compteurPointsPasses + 1].posX - (float)troncon->getTrace()[compteurPointsPasses].posX;
        float distanceTronconY = (float)troncon->getTrace()[compteurPointsPasses + 1].posY - (float)troncon->getTrace()[compteurPointsPasses].posY;
        
        /*--------------TEMPO----------------------------------------*/
        rame->setPosX(troncon->getTrace()[compteurPointsPasses].posX);
        rame->setPosY(troncon->getTrace()[compteurPointsPasses].posY);
        /*-----------------------------------------------------------*/

        float newPosX = rame->getPosX() + (distanceTronconX * pourcentageSurTroncon);
        float newPosY = rame->getPosY() + (distanceTronconY * pourcentageSurTroncon);


        //Affichage
        sf::CircleShape shape(5.f);
        shape.setFillColor(sf::Color(100, 250, 50));
        shape.setPosition(newPosX, newPosY);
        window.draw(shape);

       // cout << newPosX << ";" << newPosY << endl;
    }
}



int main()
{
	
    //Création ligne
   // gestion présence des personnes 
   //ligne n°1 : 
   //Création des stations : 
    Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, 0, false, true);
    Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, 0, false, false);
    Station Triolo("Triolo", false, 0, false, false);
    Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, 0, false, false);
    Station Pont_de_Bois("Pont de Bois", false, 0, false, false);
    Station Square_Flandres("Square Flandres", false, 0, false, false);
    Station Mairie_dHellemmes("Mairie d'Hellemmes", false, 0, false, false);
    Station Marbrerie("Marbrerie", false, 0, false, false);
    Station Fives("Fives", false, 0, false, false);
    Station Caulier("Caulier", false, 0, false, false);
    Station Gare_Lille_Flandres("Gare Lille-Flandres", false, 0, false, false);
    Station Rihour("Rihour", false, 0, false, false);
    Station République_Beaux_Arts("République - Beaux-Arts", false, 0, false, false);
    Station Gambetta("Gambetta", false, 0, false, false);
    Station Wazemmes("Wazemmes", false, 0, false, false);
    Station Porte_des_Postes("Porte des Postes", false, 0, false, false);
    Station CHU_Centre_Oscar_Lambret("CHU - Centre Oscar-Lambret", false, 0, false, false);
    Station CHU_Eurasanté("CHU - Eurasanté", false, 0, true, false);




    vector<Station*>ligne1Stations = { &Quatre_Cantons,&Cité_scientifique,&Triolo,&Villeneuve_dAscq,&Pont_de_Bois,&Square_Flandres,&Mairie_dHellemmes,&Marbrerie,&Fives,&Caulier,&Gare_Lille_Flandres,&Rihour,&République_Beaux_Arts ,&Gambetta ,&Wazemmes ,&Porte_des_Postes,&CHU_Centre_Oscar_Lambret,&CHU_Eurasanté };
    std::vector<Point2D> traceTroncon1 = { Point2D(10,20),Point2D(20,30) };
    std::vector<Point2D> traceTroncon2 = { Point2D(20,30),Point2D(40,50),Point2D(50,50)};
    Troncon ligne1_Quatre_Cantons(Quatre_Cantons, CHU_Eurasanté, 100,traceTroncon1);
    Troncon ligne1_CHU_Eurasanté(CHU_Eurasanté, Quatre_Cantons, 100,traceTroncon2);
    
    vector<Troncon*>ligne1Troncons = { &ligne1_Quatre_Cantons,&ligne1_CHU_Eurasanté };
    Ligne Ligne1 = Ligne();

    Ligne1.setListeStation(ligne1Stations);
    Ligne1.setListeTroncon(ligne1Troncons);
    vector<Ligne*> listeLignes = { &Ligne1 };

    Rame rame1 = Rame();
    rame1.setNumero(1);
    rame1.setPAX(10);
    rame1.setPositionTroncon(80);
    rame1.setTronconActuel(&ligne1_CHU_Eurasanté);

    ligne1_CHU_Eurasanté.addRameSurTroncon(rame1);

    vector<Rame*>listeRames = { &rame1 };





    Superviseur reseau = Superviseur();

    reseau.listeLignes = listeLignes;
    reseau.listeRames = listeRames;

    //---------------------Création des fenetres----------------------------------//
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "VAL");

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear();
        affichage(window,reseau);
        window.display();
    }
	



	return 0;
}
