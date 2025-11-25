#include "Genetic.h"
#include "commandline.h"
#include "LocalSearch.h"
#include "Split.h"
#include "InstanceCVRPLIB.h"

using namespace std;

int main(int argc, char *argv[])
{
	try
	{
		// Reading the arguments
		CommandLine commandline(argc, argv);

		// Força 1 veículo (TD-TSP) se não passado por argumento
		if (commandline.nbVeh == INT_MAX) commandline.nbVeh = 1;

		if (commandline.verbose) print_algorithm_parameters(commandline.ap);

		if (commandline.verbose) std::cout << "----- READING INSTANCE: " << commandline.pathInstance << std::endl;
		
		// Usa o novo Parser
		InstanceCVRPLIB inst(commandline.pathInstance);

		// Instancia Params com a nova assinatura (que definimos no passo 1)
		// Note que passamos 'inst.timeCostsTD' como último argumento
		// Passamos inst.dist_mtx[0] (tempo no intervalo 0) como matriz de distância estática base para heurísticas
		Params params(
			inst.x_coords,
			inst.y_coords,
			inst.timeCostsTD[0], // Usa intervalo 0 como referência "estática" para cálculos rápidos se necessário
			inst.service_time,
			inst.durationLimit,
			commandline.nbVeh,
			inst.isDurationConstraint,
			commandline.verbose,
			commandline.ap,
			inst.nbTimeIntervals,
			inst.intervalLength,
			inst.timeCostsTD
		);

		// Running HGS
		Genetic solver(params);
		solver.run();
		
		// Exporting
		if (solver.population.getBestFound() != NULL)
		{
			if (params.verbose) std::cout << "----- WRITING BEST SOLUTION IN : " << commandline.pathSolution << std::endl;
			solver.population.exportCVRPLibFormat(*solver.population.getBestFound(),commandline.pathSolution);
			solver.population.exportSearchProgress(commandline.pathSolution + ".PG.csv", commandline.pathInstance);
		}
	}
	catch (const string& e) { std::cout << "EXCEPTION | " << e << std::endl; }
	catch (const std::exception& e) { std::cout << "EXCEPTION | " << e.what() << std::endl; }
	return 0;
}