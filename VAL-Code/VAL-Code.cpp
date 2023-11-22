// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "troncon.hpp" 
#include "Station.h" 
#include "Rame.h"
#include <thread>
#include <mutex>


using namespace std;

int main()
{
    // Créez des objets Rame et Station
    Rame rame1(1,0,1,.0,10);
    Rame rame2(2, 10.0, 20.0, 50); // Crée une rame avec des valeurs spécifiées
    cout << "Rame 1 - Numéro: " << rame1.getNumero() << ", Position: " << rame1.getTroncon_actuelle() << endl;
    cout << "Rame 2 - Numéro: " << rame2.getNumero() << ", Position: " << rame2.getTroncon_actuelle() << endl;

    return 0;
}
