#include "Params.h"

// The universal constructor for both executable and shared library
// When the executable is run from the commandline,
// it will first generate an CVRPLIB instance from .vrp file, then supply necessary information.
Params::Params(
	const std::vector<double>& x_coords,
	const std::vector<double>& y_coords,
	const std::vector<std::vector<double>>& dist_mtx,
	const std::vector<double>& service_time,
	double durationLimit,
	int nbVeh,
	bool isDurationConstraint,
	bool verbose,
	const AlgorithmParameters& ap,
	int nbTimeIntervals_in,
	double intervalLength_in,
	const std::vector< std::vector< std::vector< double > > >& timeCostsTD_in

)
	: ap(ap), isDurationConstraint(isDurationConstraint), nbVehicles(1), durationLimit(durationLimit),
	  timeCost(dist_mtx), verbose(verbose),
      nbTimeIntervals(nbTimeIntervals_in), intervalLength(intervalLength_in), timeCostsTD(timeCostsTD_in)
{
	// This marks the starting time of the algorithm
	startTime = clock();

	nbClients = (int)service_time.size() - 1; // Need to substract the depot from the number of nodes

	// Initialize RNG
	ran.seed(ap.seed);

	// check if valid coordinates are provided
	areCoordinatesProvided = (service_time.size() == x_coords.size()) && (service_time.size() == y_coords.size());

	cli = std::vector<Client>(nbClients + 1);
	for (int i = 0; i <= nbClients; i++)
	{
		// If useSwapStar==false, x_coords and y_coords may be empty.
		if (areCoordinatesProvided)
		{
			cli[i].coordX = x_coords[i];
			cli[i].coordY = y_coords[i];
			cli[i].polarAngle = CircleSector::positive_mod(
				32768. * atan2(cli[i].coordY - cli[0].coordY, cli[i].coordX - cli[0].coordX) / PI);
		}
		else
		{
			cli[i].coordX = 0.0;
			cli[i].coordY = 0.0;
			cli[i].polarAngle = 0.0;
		}

		cli[i].serviceDuration = service_time[i];
	}

	// Calculation of the maximum distance
	maxDist = 0.;
	for (int i = 0; i <= nbClients; i++)
		for (int j = 0; j <= nbClients; j++)
			if (timeCost[i][j] > maxDist) maxDist = timeCost[i][j];

	// Calculation of the correlated vertices for each customer (for the granular restriction)
	correlatedVertices = std::vector<std::vector<int> >(nbClients + 1);
	std::vector<std::set<int> > setCorrelatedVertices = std::vector<std::set<int> >(nbClients + 1);
	std::vector<std::pair<double, int> > orderProximity;
	for (int i = 1; i <= nbClients; i++)
	{
		orderProximity.clear();
		for (int j = 1; j <= nbClients; j++)
			if (i != j) orderProximity.emplace_back(timeCost[i][j], j);
		std::sort(orderProximity.begin(), orderProximity.end());

		for (int j = 0; j < std::min<int>(ap.nbGranular, nbClients - 1); j++)
		{
			// If i is correlated with j, then j should be correlated with i
			setCorrelatedVertices[i].insert(orderProximity[j].second);
			setCorrelatedVertices[orderProximity[j].second].insert(i);
		}
	}

	// Filling the vector of correlated vertices
	for (int i = 1; i <= nbClients; i++)
		for (int x : setCorrelatedVertices[i])
			correlatedVertices[i].push_back(x);

	// Safeguards to avoid possible numerical instability in case of instances containing arbitrarily small or large numerical values
	if (maxDist < 0.1 || maxDist > 100000)
		throw std::string(
			"The distances are of very small or large scale. This could impact numerical stability. Please rescale the dataset and run again.");

	// A reasonable scale for the initial values of the penalties
	penaltyDuration = 1;

	if (verbose)
		std::cout << "----- INSTANCE SUCCESSFULLY LOADED WITH " << nbClients << " CLIENTS AND " << nbVehicles << " VEHICLES" << std::endl;
}

double Params::getTDCost(int i, int j, double timeDeparture) const
{
    // Lógica do Link Travel Model (LTM)
    
    // 1. Determinar o intervalo de tempo h de partida (indexação de tempo discreto)
    // Usamos floor para garantir que o tempo de partida caia no intervalo correto.
    int h = (int)std::floor(timeDeparture / intervalLength);
    
    // 2. Limitar h ao número máximo de intervalos (o último intervalo é o default se o tempo exceder o limite)
    h = std::min(h, nbTimeIntervals - 1); 

    // 3. Retorna o custo do arco (i, j) para aquele intervalo h.
    return timeCostsTD[h][i][j];
}

