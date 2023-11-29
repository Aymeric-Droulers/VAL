
#include <vector>
#include "Station.h"


/*Cette classe représente un troncon. Un troncon relie deux stations.
Plusieurs troncons forment une ligne */

class Troncon {
private:
	Station stationDebut;
	Station stationFin;
	int tailleTroncon;
	std::vector<Rame> ramesSurTroncon;
public:
	Troncon(Station debut, Station fin, int taille, vector<Rame> stationSurTroncon)
		: stationDebut(debut), stationFin(fin), tailleTroncon(taille), ramesSurTroncon(stationSurTroncon) {
		}
	Station getStationDebut() { return (this->stationDebut); }
	Station getStationFin() { return(this->stationFin); }
	int getTailleTroncon() { return this->tailleTroncon; }
	std::vector<Rame> getRamesSurLigne() { return this->ramesSurTroncon; }

	void setStationDebut(const Station&);
	void setStationFin(const Station&);
	void setTailleTroncon(int taille);
	void setRamesSurTroncon(const std::vector<Rame>);
	void addRameSurTroncon(const Rame&);
	void removeRameSurLigne(int rameId);
};