#include "Individual.h" 

void Individual::evaluateCompleteCost(const Params & params)
{
	eval = EvalIndiv();

	// Como é um TSP (1 veículo), ignoramos a lógica de Split complexa do CVRP.
	// O Giant Tour (chromT) é a própria rota do único veículo.
	chromR.clear();
	chromR.resize(params.nbVehicles); // Deve ser 1
	chromR[0] = chromT; // Copia a sequência para a estrutura de rotas
	
	// Reinicializa predecessores e sucessores
	for(int i = 0; i <= params.nbClients; i++) {
		predecessors[i] = 0;
		successors[i] = 0;
	}// Verifica se a rota não está vazia
	if (!chromR[0].empty())
	{
		double currentTime = 0.0; // Início do horizonte de planejamento (t0)
		int previousNode = 0;     // Começa no Depósito

		// Itera sequencialmente sobre a rota para calcular o MakeSpan (Tempo Total)
		// Isso é mandatório no TD-TSP devido à propriedade de não-aditividade
		for (int currentNode : chromR[0])
		{
			// 1. Custo de viagem (Dependente do tempo de saída de previousNode)
			double travelTime = params.getTDCost(previousNode, currentNode, currentTime);
			currentTime += travelTime;

			// 2. Tempo de serviço no cliente
			currentTime += params.cli[currentNode].serviceDuration;

			// Atualiza estruturas de navegação
			successors[previousNode] = currentNode;
			predecessors[currentNode] = previousNode;

			previousNode = currentNode;
		}

		// Retorno ao depósito
		double returnTime = params.getTDCost(previousNode, 0, currentTime);
		currentTime += returnTime;
		
		successors[previousNode] = 0;
		predecessors[0] = previousNode;

		// O "custo" principal agora é o tempo total da rota (Makespan)
		eval.distance = currentTime; 
		eval.nbRoutes = 1;

		// Verificação de violação do Horizonte de Planejamento (Duration Constraint)
		// Assumindo que durationLimit é o tempo máximo permitido (ex: fim do dia)
		if (params.isDurationConstraint && eval.distance > params.durationLimit)
		{
			eval.durationExcess = eval.distance - params.durationLimit;
		}
	}

	// Custo Penalizado (Fitness)
	// Como não há capacidade, removemos params.penaltyCapacity
	eval.penalizedCost = eval.distance + eval.durationExcess * params.penaltyDuration;
	
	// É viável se não violar o horizonte de tempo (dentro da tolerância epsilon)
	eval.isFeasible = (eval.durationExcess < MY_EPSILON);
}

Individual::Individual(Params & params)
{
	successors = std::vector <int>(params.nbClients + 1);
	predecessors = std::vector <int>(params.nbClients + 1);
	chromR = std::vector < std::vector <int> >(params.nbVehicles);
	chromT = std::vector <int>(params.nbClients);
	
	// Preenche o cromossomo com todos os clientes
	for (int i = 0; i < params.nbClients; i++) chromT[i] = i + 1;
	
	// Embaralha aleatoriamente para gerar solução inicial
	std::shuffle(chromT.begin(), chromT.end(), params.ran);
	
	// Inicializa custo com valor alto antes da primeira avaliação
	eval.penalizedCost = 1.e30; 
	
	// Avalia imediatamente para garantir consistência
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
		// Loops in the input file as long as the first line keyword is "Route"
		for (int r = 0; inputString == "Route"; r++)
		{
			inputFile >> inputString;
			getline(inputFile, inputString);
			std::stringstream ss(inputString);
			int inputCustomer;
			while (ss >> inputCustomer) // Loops as long as there is an integer to read in this route
			{
				chromT.push_back(inputCustomer);
				// Nota: chromR será reconstruído corretamente dentro de evaluateCompleteCost
			}
			inputFile >> inputString;
		}
		if (inputString == "Cost") inputFile >> readCost;
		else throw std::string("Unexpected token in input solution");

		// Verificações de segurança e recálculo
		evaluateCompleteCost(params);
		
		if ((int)chromT.size() != params.nbClients) throw std::string("Input solution does not contain the correct number of clients");
		if (!eval.isFeasible) throw std::string("Input solution is infeasible");
		
		// Nota: O custo lido do arquivo pode diferir ligeiramente devido à precisão de ponto flutuante
		// ou se o arquivo original não considerava a dependência de tempo da mesma forma.
		if (std::abs(eval.penalizedCost - readCost) > 1.0 && params.verbose) 
			std::cout << "WARNING: Input solution cost differs from recalculated TD-TSP cost." << std::endl;
			
		if (params.verbose) std::cout << "----- INPUT SOLUTION HAS BEEN SUCCESSFULLY READ WITH COST " << eval.penalizedCost << std::endl;
	}
	else 
		throw std::string("Impossible to open solution file provided in input in : " + fileName);
}