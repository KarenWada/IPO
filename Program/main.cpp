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

		// Instancia Params com a nova assinatura (14 argumentos)
		Params params(
			inst.x_coords,
			inst.y_coords,
			inst.timeCostsTD[0], // Usa intervalo 0 como referência "estática" para dist_mtx
			inst.service_time,
			inst.demands,        // Novo argumento: Demands
			inst.vehicleCapacity,
			inst.durationLimit,
			commandline.nbVeh,
			inst.isDurationConstraint,
			commandline.verbose,
			commandline.ap,
			inst.nbTimeIntervals, // Novo argumento: TD Info
			inst.intervalLength,  // Novo argumento: TD Info
			inst.timeCostsTD      // Novo argumento: TD Matrix
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