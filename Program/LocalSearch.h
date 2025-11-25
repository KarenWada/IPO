/*MIT License
Copyright(c) 2020 Thibaut Vidal
*/

#ifndef LOCALSEARCH_H
#define LOCALSEARCH_H

#include "Individual.h"
#include <vector>
#include <algorithm>
#include <cmath>
#include <set>

struct Node;

// Structure containing a route
struct Route
{
	int cour;							// Route index
	int nbCustomers;					// Number of customers visited in the route
	Node * depot;						// Pointer to the associated depot
	double duration;					// Total time on the route (Objective Function)
	double penalty;						// Current sum of duration penalties
};

struct Node
{
	bool isDepot;						// Tells whether this node represents a depot or not
	int cour;							// Node index
	int position;						// Position in the route
	Node * next;						// Next node in the route order
	Node * prev;						// Previous node in the route order
	Route * route;						// Pointer towards the associated route
};

// Main local search structure
class LocalSearch
{

private:
	
	Params & params;							
	bool searchCompleted;						
	int nbMoves;								
	std::vector < int > orderNodes;				
	int loopID;									

	/* THE SOLUTION IS REPRESENTED AS A LINKED LIST OF ELEMENTS */
	std::vector < Node > clients;				
	std::vector < Node > depots;				
	std::vector < Node > depotsEnd;				
	std::vector < Route > routes;				

	/* TEMPORARY VARIABLES */
	Node * nodeU;
	Node * nodeV;
	Node * nodeX;
	Node * nodeY;
	Route * routeU; // In TSP, routeU is always equal to routeV
	
	double penaltyDurationLS;

	// Helper to set up neighborhood search variables
	void setLocalVariablesRouteU(); 
	void setLocalVariablesRouteV(); 

	// CORE TD-TSP FUNCTION: Calculates the full duration of a route by traversing it
	double calculateRouteDuration(Route* route);

	/* RELOCATE MOVES */
	bool move1(); // Relocate Node U after Node V

	/* SWAP MOVES */
	bool move4(); // Swap Node U and Node V
	 
	/* 2-OPT MOVES */
	bool move7(); // 2-Opt (Reverse segment between U and V)

	/* ROUTINES TO UPDATE THE SOLUTIONS */
	static void insertNode(Node * U, Node * V);		// Insert U after V
	static void swapNode(Node * U, Node * V);		// Swap U and V
	static void reversePath(Node * start, Node * end); // Reverse path for 2-opt
	void updateRouteData(Route * myRoute);			// Updates route duration

public:

	void run(Individual & indiv, double penaltyCapacityLS, double penaltyDurationLS);
	void loadIndividual(const Individual & indiv);
	void exportIndividual(Individual & indiv);

	LocalSearch(Params & params);
};

#endif