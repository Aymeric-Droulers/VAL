// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "Rame.hpp"
#include "Station.h"
#include "troncon.hpp"
using namespace std;

int main()
{
	
	cout << "Hello CMake." << endl;
	Troncon t1 = Troncon();
	Rame rame1 = Rame();
	rame1.setNumero(1);
	Rame rame2 = Rame();
	rame2.setNumero(2);

	cout << "Rame1: " << rame1.getNumero() << endl;
	cout << "Rame2: " << rame2.getNumero() << endl;
	
	
	t1.addRameSurTroncon(rame1);
	cout << t1.getRamesSurLigne().size() << endl;
	t1.removeRameSurLigne(1);

	cout << t1.getRamesSurLigne().size() << endl;

	

	return 0;
}
