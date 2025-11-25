#include "Split.h" 

Split::Split(const Params & params) : params(params)
{
	// No initialization needed for TD-TSP as complex split structures are removed.
}

void Split::generalSplit(Individual & indiv, int nbMaxVehicles)
{
	// In a standard CVRP, this function would use dynamic programming (Split algorithm)
	// to optimally segment the Giant Tour into feasible vehicle routes.
	
	// For the TD-TSP (Time-Dependent TSP), we have a fixed fleet of 1 vehicle 
	// and no capacity constraints. Therefore, the "Giant Tour" IS the solution.
	
	// We simply call evaluateCompleteCost. 
	// In our modified Individual.cpp, this function will:
	// 1. Assign the entire chromT to chromR[0] (single vehicle).
	// 2. Calculate the time-dependent cost sequentially.
	
	indiv.evaluateCompleteCost(params);
}