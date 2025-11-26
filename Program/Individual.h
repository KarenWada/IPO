/*MIT License
Copyright(c) 2020 Thibaut Vidal
*/

#ifndef INDIVIDUAL_H
#define INDIVIDUAL_H

#include "Params.h"

struct EvalIndiv
{
	double penalizedCost = 0.;		// Custo total com penalidades (Função Objetivo do Algoritmo)
	double travelTime = 0.;         // APENAS o tempo gasto viajando (Para comparar com o Artigo)
	
	int nbRoutes = 0;				
	double distance = 0.;			// Makespan (Tempo Total: Viagem + Serviço)
	double durationExcess = 0.;		// Violação do horizonte de tempo
	bool isFeasible = false;		
};

class Individual
{
public:

  EvalIndiv eval;															
  std::vector < int > chromT ;												
  std::vector < std::vector <int> > chromR ;								
  std::vector < int > successors ;											
  std::vector < int > predecessors ;										
  std::multiset < std::pair < double, Individual* > > indivsPerProximity ;	
  double biasedFitness;														

  void evaluateCompleteCost(const Params & params);

  Individual(Params & params);

  Individual(Params & params, std::string fileName);
};
#endif