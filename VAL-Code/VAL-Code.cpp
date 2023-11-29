// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "troncon.hpp" 
#include "Station.h" 
#include "Rame.h"
#include <thread>
#include <mutex>


using namespace std;


int rand(int size) {
    if (size == 1) {
        int borneInferieure = 1;
        int borneSuperieure = 10;
        int nombreAleatoire = borneInferieure + rand() % (borneSuperieure - borneInferieure + 1);
        return nombreAleatoire;
    };
    if (size == 2) {
        int borneInferieure = 10;
        int borneSuperieure = 30;
        int nombreAleatoire = borneInferieure + rand() % (borneSuperieure - borneInferieure + 1);
        return nombreAleatoire;

    };
    if (size == 3) {
        int borneInferieure = 30;
        int borneSuperieure = 50;
        int nombreAleatoire = borneInferieure + rand() % (borneSuperieure - borneInferieure + 1);
        return nombreAleatoire;
    };
}

int main()
{
    // Créez des objets Rame et Station
    Rame rame1(1,0,1,.0,10);
    Rame rame2(2, 10.0, 20.0, 50); // Crée une rame avec des valeurs spécifiées
    cout << "Rame 1 - Numéro: " << rame1.getNumero() << ", Position: " << rame1.getTroncon_actuelle() << endl;
    cout << "Rame 2 - Numéro: " << rame2.getNumero() << ", Position: " << rame2.getTroncon_actuelle() << endl;
    //Création ligne
    // gestion présence des personnes 
    //ligne n°1 : 
    //Création des stations : 
    Station Quatre_Cantons("Quatre_Cantons - Stade Pierre - Mauroy", false, rand(1), false, true);
    Station Cité_scientifique("Cité scientifique - Professeur Gabillard", false, rand(1), false, false);
    Station Triolo("Triolo", false, rand(1), false, false);
    Station Villeneuve_dAscq("Villeneuve-d'Ascq - Hôtel de Ville", false, rand(1), false, false);
    Station Pont_de_Bois("Pont de Bois", false, rand(1), false, false);
    Station Square_Flandres("Square Flandres", false, rand(1), false, false);
    Station Mairie_dHellemmes("Mairie d'Hellemmes", false, rand(1), false, false);
    Station Marbrerie("Marbrerie", false, rand(1), false, false);
    Station Fives("Fives", false, rand(1), false, false);
    Station Caulier("Caulier", false, rand(1), false, false);
    Station Gare_Lille_Flandres("Gare Lille-Flandres", false, rand(1), false, false);
    Station Rihour("Rihour", false, rand(1), false, false);
    Station République_Beaux_Arts("République - Beaux-Arts", false, rand(1), false, false);
    Station Gambetta("Gambetta", false, rand(1), false, false);
    Station Wazemmes("Wazemmes", false, rand(1), false, false);
    Station Porte_des_Postes("Porte des Postes", false, rand(1), false, false);
    Station CHU_Centre_Oscar_Lambret("CHU - Centre Oscar-Lambret", false, rand(1), false, false);
    Station CHU_Eurasanté("CHU - Eurasanté", false, rand(1), true, false);




    vector<Rame>ligne1 = { Quatre_Cantons,Cité_scientifique,Triolo,Villeneuve_dAscq,Pont_de_Bois,Square_Flandres,Mairie_dHellemmes,Marbrerie,Fives,Caulier,Gare_Lille_Flandres,Rihour,République_Beaux_Arts ,Gambetta ,Wazemmes ,Porte_des_Postes,CHU_Centre_Oscar_Lambret,CHU_Eurasanté };
    
    Troncon ligne1_Quatre_Cantons(Quatre_Cantons, CHU_Eurasanté, 100, ligne1);
    Troncon ligne1_CHU_Eurasanté(CHU_Eurasanté, Quatre_Cantons, 100, ligne1);


    return 0;
}
