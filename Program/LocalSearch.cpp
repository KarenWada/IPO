#include "LocalSearch.h" 

void LocalSearch::run(Individual & indiv, double penaltyCapacityLS, double penaltyDurationLS)
{
	this->penaltyDurationLS = penaltyDurationLS;
	loadIndividual(indiv);

	// Shuffling
	std::shuffle(orderNodes.begin(), orderNodes.end(), params.ran);

	searchCompleted = false;

	
    // ADICIONE ESTE CONTADOR
    int debugCount = 0;

	for (loopID = 0; !searchCompleted; loopID++)
	{
        // ADICIONE ESTA VERIFICAÇÃO DE SEGURANÇA
        debugCount++;
        if (debugCount > 1000) {
            std::cout << "ALERTA: Local Search em loop infinito?" << std::endl;
            break; }
		searchCompleted = true;
    }

		
	
	// Main LS Loop
	// We simplify the logic to standard Hill Climbing / Descent
	for (loopID = 0; !searchCompleted; loopID++)
	{
		searchCompleted = true;

		for (int posU = 0; posU < params.nbClients; posU++)
		{
			nodeU = &clients[orderNodes[posU]];
			
			// Granular Search: check neighbors
			// In TD-TSP, static correlation is an approximation, but useful for speed
			for (int posV = 0; posV < (int)params.correlatedVertices[nodeU->cour].size(); posV++)
			{
				nodeV = &clients[params.correlatedVertices[nodeU->cour][posV]];
				
				// Skip if same node
				if (nodeU == nodeV) continue;

				setLocalVariablesRouteU();
				setLocalVariablesRouteV();

				// Try Moves
				// Only attempting improvement if it respects logical constraints
				
				// Relocate U after V
				if (nodeU->next != nodeV && nodeU->prev != nodeV) {
					if (move1()) continue; 
				}

				// Swap U and V
				if (nodeU != nodeV && nodeU->next != nodeV) { // Avoid adjacent swap in move4 for simplicity (covered by 2opt or specific logic)
					if (move4()) continue; 
				}

				// 2-Opt (Standard TSP move)
				if (nodeU->position < nodeV->position && nodeU->next != nodeV) {
					if (move7()) continue;
				}
			}
		}
	}

	exportIndividual(indiv);
}

// -----------------------------------------------------------------------------
// CORE EVALUATION LOGIC FOR TD-TSP
// -----------------------------------------------------------------------------
double LocalSearch::calculateRouteDuration(Route* route)
{
	double currentTime = 0.0; // Start at T=0
	Node* curr = route->depot;
	
	// Traverse the linked list
	while (!curr->next->isDepot)
	{
		int from = curr->cour;
		int to = curr->next->cour;
		
		// 1. Travel Time (Time Dependent)
		double travelTime = params.getTDCost(from, to, currentTime);
		currentTime += travelTime;

		// 2. Service Time
		currentTime += params.cli[to].serviceDuration;

		curr = curr->next;
	}

	// Return to depot
	int from = curr->cour;
	int to = 0; // Depot
	double returnTime = params.getTDCost(from, to, currentTime);
	currentTime += returnTime;

	return currentTime;
}

// -----------------------------------------------------------------------------
// MOVES IMPLEMENTATION (Apply -> Evaluate -> Revert)
// -----------------------------------------------------------------------------

bool LocalSearch::move1() // RELOCATE U after V
{
	// 1. Backup state to allow revert
	Node* u_prev = nodeU->prev;
	Node* u_next = nodeU->next;
	Node* v_next = nodeV->next;
	double oldCost = routeU->duration; // Objective: Minimize Duration

	// 2. Apply Move
	insertNode(nodeU, nodeV); // Helper puts U after V

	// 3. Full Evaluation (O(n))
	double newCost = calculateRouteDuration(routeU);
	
	// Penalties (if constraint is strict)
	double newPenalty = std::max<double>(0., newCost - params.durationLimit) * penaltyDurationLS;
	double currentTotal = oldCost + routeU->penalty;
	double newTotal = newCost + newPenalty;

	// 4. Check Improvement
	if (newTotal < currentTotal - MY_EPSILON)
	{
		// Keep move
		updateRouteData(routeU);
		nbMoves++;
		searchCompleted = false;
		return true;
	}
	else
	{
		// 5. Revert Move
		// To revert "U after V", we place U back after u_prev
		insertNode(nodeU, u_prev);
		return false;
	}
}

bool LocalSearch::move4() // SWAP U and V
{
	// 1. Backup
	Node* u_prev = nodeU->prev; // We rely on swapNode logic correctness, but backup is safer
	Node* v_prev = nodeV->prev;
	double oldCost = routeU->duration;

	// 2. Apply
	swapNode(nodeU, nodeV);

	// 3. Evaluate
	double newCost = calculateRouteDuration(routeU);
	
	double newPenalty = std::max<double>(0., newCost - params.durationLimit) * penaltyDurationLS;
	double currentTotal = oldCost + routeU->penalty;
	double newTotal = newCost + newPenalty;

	// 4. Check
	if (newTotal < currentTotal - MY_EPSILON)
	{
		updateRouteData(routeU);
		nbMoves++;
		searchCompleted = false;
		return true;
	}
	else
	{
		// 5. Revert (Swapping again restores positions)
		swapNode(nodeU, nodeV); 
		return false;
	}
}

bool LocalSearch::move7() // 2-OPT (Reverse segment between U and V)
{
	// U is start of segment, V is end of segment.
	// In standard HGS 2-opt, we swap edges (u_prev, u) and (v, v_next) with (u_prev, v) and (u, v_next)
	// Effectively reversing the chain u...v
	
	double oldCost = routeU->duration;

	// 2. Apply Move: Reverse path from U to V
	reversePath(nodeU, nodeV);

	// 3. Evaluate
	double newCost = calculateRouteDuration(routeU);

	double newPenalty = std::max<double>(0., newCost - params.durationLimit) * penaltyDurationLS;
	double currentTotal = oldCost + routeU->penalty;
	double newTotal = newCost + newPenalty;

	// 4. Check
	if (newTotal < currentTotal - MY_EPSILON)
	{
		updateRouteData(routeU);
		nbMoves++;
		searchCompleted = false;
		return true;
	}
	else
	{
		// 5. Revert: Reverse again restores order
		reversePath(nodeV, nodeU); // Reversing the now inverted segment
		return false;
	}
}

// -----------------------------------------------------------------------------
// HELPERS
// -----------------------------------------------------------------------------

void LocalSearch::insertNode(Node * U, Node * V)
{
	// Remove U from current position
	U->prev->next = U->next;
	U->next->prev = U->prev;
	
	// Insert U after V
	V->next->prev = U;
	U->next = V->next;
	V->next = U;
	U->prev = V;
}

void LocalSearch::swapNode(Node * U, Node * V)
{
	// Standard pointer swap logic for doubly linked list
	Node * myVPred = V->prev;
	Node * myVSuiv = V->next;
	Node * myUPred = U->prev;
	Node * myUSuiv = U->next;

	// Handle adjacent case if necessary, but loop filters likely handle it.
	// Assuming non-adjacent for simplicity of the backup logic
	
	myUPred->next = V;
	myUSuiv->prev = V;
	myVPred->next = U;
	myVSuiv->prev = U;

	U->prev = myVPred;
	U->next = myVSuiv;
	V->prev = myUPred;
	V->next = myUSuiv;
}

void LocalSearch::reversePath(Node * start, Node * end)
{
	// Reverses the chain of nodes from start to end (inclusive)
	// Connects start->prev to end, and end->next to start
	
	Node* predWrapper = start->prev;
	Node* succWrapper = end->next;
	
	Node* curr = start;
	Node* nextNode = NULL;
	Node* prevNode = succWrapper; // The first node (start) will point to succWrapper as next

	// Simple reversal logic
	// We need to be careful with the loop.
	// Let's use a vector to store the nodes to avoid breaking links while iterating
	// (Slow but safe for prototyping)
	std::vector<Node*> segment;
	curr = start;
	while(curr != end) {
		segment.push_back(curr);
		curr = curr->next;
	}
	segment.push_back(end);

	// Re-link internal connections reversed
	for(size_t i = 0; i < segment.size(); i++) {
		Node* n = segment[i];
		n->next = (i == 0) ? predWrapper : segment[i-1];
		n->prev = (i == segment.size()-1) ? succWrapper : segment[i+1];
	}

	// Fix outer connections
	predWrapper->next = end;
	succWrapper->prev = start;

	// Note: The logic above reverses the segment physically. 
	// Start is now physically near succWrapper. End is near predWrapper.
}

void LocalSearch::setLocalVariablesRouteU()
{
	routeU = nodeU->route;
}

void LocalSearch::setLocalVariablesRouteV()
{
	// For TSP, routeV is same as routeU
}

void LocalSearch::updateRouteData(Route * myRoute)
{
	// Just update the cost
	double cost = calculateRouteDuration(myRoute);
	myRoute->duration = cost;
	myRoute->penalty = std::max<double>(0., cost - params.durationLimit) * penaltyDurationLS;
}

void LocalSearch::loadIndividual(const Individual & indiv)
{
	nbMoves = 0; 
	// TSP: Load Single Route 0
	int r = 0;
	
	Node * myDepot = &depots[r];
	Node * myDepotFin = &depotsEnd[r];
	Route * myRoute = &routes[r];
	
	myDepot->prev = myDepotFin;
	myDepotFin->next = myDepot;
	
	if (!indiv.chromR[r].empty())
	{
		Node * myClient = &clients[indiv.chromR[r][0]];
		myClient->route = myRoute;
		myClient->prev = myDepot;
		myDepot->next = myClient;
		for (int i = 1; i < (int)indiv.chromR[r].size(); i++)
		{
			Node * myClientPred = myClient;
			myClient = &clients[indiv.chromR[r][i]]; 
			myClient->prev = myClientPred;
			myClientPred->next = myClient;
			myClient->route = myRoute;
		}
		myClient->next = myDepotFin;
		myDepotFin->prev = myClient;
	}
	else
	{
		myDepot->next = myDepotFin;
		myDepotFin->prev = myDepot;
	}
	
	updateRouteData(myRoute);
}

void LocalSearch::exportIndividual(Individual & indiv)
{
	// Convert Linked List back to Chromosome
	indiv.chromT.clear();
	indiv.chromR[0].clear(); // TSP Single Route
	
	Node * node = depots[0].next;
	while (!node->isDepot)
	{
		indiv.chromT.push_back(node->cour);
		indiv.chromR[0].push_back(node->cour);
		node = node->next;
	}

	indiv.evaluateCompleteCost(params);
}

LocalSearch::LocalSearch(Params & params) : params (params)
{
	clients = std::vector < Node >(params.nbClients + 1);
	routes = std::vector < Route >(params.nbVehicles);
	depots = std::vector < Node >(params.nbVehicles);
	depotsEnd = std::vector < Node >(params.nbVehicles);

	for (int i = 0; i <= params.nbClients; i++) 
	{ 
		clients[i].cour = i; 
		clients[i].isDepot = false; 
	}
	for (int i = 0; i < params.nbVehicles; i++)
	{
		routes[i].cour = i;
		routes[i].depot = &depots[i];
		depots[i].cour = 0;
		depots[i].isDepot = true;
		depots[i].route = &routes[i];
		depotsEnd[i].cour = 0;
		depotsEnd[i].isDepot = true;
		depotsEnd[i].route = &routes[i];
	}
	for (int i = 1 ; i <= params.nbClients ; i++) orderNodes.push_back(i);
}