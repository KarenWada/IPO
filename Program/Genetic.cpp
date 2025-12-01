#include "Genetic.h"

void Genetic::run()
{	
	/* INITIAL POPULATION */
	population.generatePopulation();

	int nbIter;
	int nbIterNonProd = 1;
	if (params.verbose) std::cout << "----- STARTING GENETIC ALGORITHM" << std::endl;
	
	// Condição de parada: número de iterações sem melhoria ou tempo limite
	for (nbIter = 0 ; 
		 nbIterNonProd <= params.ap.nbIter && 
		 (params.ap.timeLimit == 0 || (double)(clock()-params.startTime)/(double)CLOCKS_PER_SEC < params.ap.timeLimit) ; 
		 nbIter++)
	{	
		// --- NOVO: VERIFICAÇÃO DE ALVO (TARGET) ---
		// Se definimos um alvo e a melhor solução viável encontrada já é menor ou igual a ele, paramos!
		if (params.ap.targetTravelTime > 0.1)
		{
			const Individual* best = population.getBestFound();
			// Usamos uma pequena tolerância (+0.001) para erros de ponto flutuante
			if (best && best->eval.isFeasible && best->eval.travelTime <= params.ap.targetTravelTime + 0.001) 
			{
				if (params.verbose) std::cout << "----- TARGET REACHED: " << best->eval.travelTime << " <= " << params.ap.targetTravelTime << std::endl;
				break; // Sai do loop imediatamente
			}
		}
		// ------------------------------------------

		/* SELECTION AND CROSSOVER */
		crossoverOX(offspring, population.getBinaryTournament(),population.getBinaryTournament());

		/* LOCAL SEARCH */
		// Passamos 0.0 para capacidade (irrelevante para TSP)
		localSearch.run(offspring, 0.0, params.penaltyDuration);
		
		bool isNewBest = population.addIndividual(offspring,true);
		
		// Lógica de reparo baseada apenas na viabilidade de tempo/duração
		if (!offspring.eval.isFeasible && params.ran()%2 == 0) // Repara metade das soluções inviáveis
		{
			// Aumenta penalidade para forçar viabilidade
			localSearch.run(offspring, 0.0, params.penaltyDuration*10.);
			if (offspring.eval.isFeasible) isNewBest = (population.addIndividual(offspring,false) || isNewBest);
		}

		/* TRACKING THE NUMBER OF ITERATIONS SINCE LAST SOLUTION IMPROVEMENT */
		if (isNewBest) nbIterNonProd = 1;
		else nbIterNonProd ++ ;

		/* DIVERSIFICATION, PENALTY MANAGEMENT AND TRACES */
		if (nbIter % params.ap.nbIterPenaltyManagement == 0) population.managePenalties();
		if (nbIter % params.ap.nbIterTraces == 0) population.printState(nbIter, nbIterNonProd);

		/* RESTART (Reinício da população para escapar de ótimos locais) */
		if (params.ap.timeLimit != 0 && nbIterNonProd == params.ap.nbIter)
		{
			population.restart();
			nbIterNonProd = 1;
		}
	}
	if (params.verbose) std::cout << "----- GENETIC ALGORITHM FINISHED AFTER " << nbIter << " ITERATIONS. TIME SPENT: " << (double)(clock() - params.startTime) / (double)CLOCKS_PER_SEC << std::endl;
}

void Genetic::crossoverOX(Individual & result, const Individual & parent1, const Individual & parent2)
{
	// Frequency table to track the customers which have been already inserted
	std::vector <bool> freqClient = std::vector <bool> (params.nbClients + 1, false);

	// Picking the beginning and end of the crossover zone
	std::uniform_int_distribution<> distr(0, params.nbClients-1);
	int start = distr(params.ran);
	int end = distr(params.ran);

	// Avoid that start and end coincide by accident
	while (end == start) end = distr(params.ran);

	// Copy from start to end
	int j = start;
	while (j % params.nbClients != (end + 1) % params.nbClients)
	{
		result.chromT[j % params.nbClients] = parent1.chromT[j % params.nbClients];
		freqClient[result.chromT[j % params.nbClients]] = true;
		j++;
	}

	// Fill the remaining elements in the order given by the second parent
	for (int i = 1; i <= params.nbClients; i++)
	{
		int temp = parent2.chromT[(end + i) % params.nbClients];
		if (freqClient[temp] == false)
		{
			result.chromT[j % params.nbClients] = temp;
			j++;
		}
	}

	// Complete the individual with the Split algorithm
	// In TD-TSP, this effectively just evaluates the cost of the single route
	split.generalSplit(result, params.nbVehicles);
}

Genetic::Genetic(Params & params) : 
	params(params), 
	split(params),
	localSearch(params),
	population(params,this->split,this->localSearch),
	offspring(params){}