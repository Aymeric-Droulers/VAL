// VAL-Code.cpp : définit le point d'entrée de l'application.
//

#include "VAL-Code.h"
#include "Rame.h"

using namespace std;

int main()
{
	cout << "Hello CMake." << endl;
	
	Rame rame1 = Rame();
	rame1.setNumero(1);
	Rame rame2 = Rame();
	rame2.setNumero(2);

	cout << "Rame1: " << rame1.getNumero() << endl;
	cout << "Rame2: " << rame2.getNumero() << endl;


	

	return 0;
}
