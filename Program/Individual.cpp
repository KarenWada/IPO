#include "Individual.h" 

void Individual::evaluateCompleteCost(const Params & params)
{
	eval = EvalIndiv();
	
	chromR.clear();
	chromR.resize(params.nbVehicles); 
	chromR[0] = chromT; 

	for(int i = 0; i <= params.nbClients; i++) {
		predecessors[i] = 0;
		successors[i] = 0;
	}

	if (!chromR[0].empty())
	{
		double currentTime = 0.0; 
		int previousNode = 0;     
		double sumTravelTime = 0.0; // Acumulador de tempo de viagem puro

		for (int currentNode : chromR[0])
		{
			double travelTime = params.getTDCost(previousNode, currentNode, currentTime);
			
			sumTravelTime += travelTime; // Soma apenas viagem
			currentTime += travelTime;   // Relógio avança com viagem

			currentTime += params.cli[currentNode].serviceDuration; // Relógio avança com serviço

			successors[previousNode] = currentNode;
			predecessors[currentNode] = previousNode;

			previousNode = currentNode;
		}

		// Retorno ao depósito
		double returnTime = params.getTDCost(previousNode, 0, currentTime);
		sumTravelTime += returnTime;
		currentTime += returnTime;
		
		successors[previousNode] = 0;
		predecessors[0] = previousNode;

		eval.travelTime = sumTravelTime; // Métrica do Artigo
		eval.distance = currentTime;     // Métrica do HGS (Makespan)
		eval.nbRoutes = 1;

		if (params.isDurationConstraint && eval.distance > params.durationLimit)
		{
			eval.durationExcess = eval.distance - params.durationLimit;
		}
	}

	// Custo Penalizado para guiar a busca
	eval.penalizedCost = eval.distance + eval.durationExcess * params.penaltyDuration;
	
	eval.isFeasible = (eval.durationExcess < MY_EPSILON);
}

Individual::Individual(Params & params)
{
	successors = std::vector <int>(params.nbClients + 1);
	predecessors = std::vector <int>(params.nbClients + 1);
	chromR = std::vector < std::vector <int> >(params.nbVehicles);
	chromT = std::vector <int>(params.nbClients);
	
	for (int i = 0; i < params.nbClients; i++) chromT[i] = i + 1;
	std::shuffle(chromT.begin(), chromT.end(), params.ran);
	
	eval.penalizedCost = 1.e30; 
	evaluateCompleteCost(params);
}

Individual::Individual(Params & params, std::string fileName) : Individual(params)
{
	double readCost;
	chromT.clear();
	std::ifstream inputFile(fileName);
	if (inputFile.is_open())
	{
		std::string inputString;
		inputFile >> inputString;
		for (int r = 0; inputString == "Route"; r++)
		{
			inputFile >> inputString;
			getline(inputFile, inputString);
			std::stringstream ss(inputString);
			int inputCustomer;
			while (ss >> inputCustomer) 
			{
				chromT.push_back(inputCustomer);
			}
			inputFile >> inputString;
		}
		if (inputString == "Cost") inputFile >> readCost;
		else throw std::string("Unexpected token in input solution");

		evaluateCompleteCost(params);
		
		if ((int)chromT.size() != params.nbClients) throw std::string("Input solution does not contain the correct number of clients");
		if (!eval.isFeasible) throw std::string("Input solution is infeasible");
		
		if (params.verbose) std::cout << "----- INPUT SOLUTION HAS BEEN SUCCESSFULLY READ WITH COST " << eval.penalizedCost << std::endl;
	}
	else 
		throw std::string("Impossible to open solution file provided in input in : " + fileName);
}