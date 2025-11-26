#include "InstanceCVRPLIB.h"
#include <cmath>
#include <fstream>
#include <iostream>

InstanceCVRPLIB::InstanceCVRPLIB(std::string pathToInstance)
{
	std::ifstream inputFile(pathToInstance);
	if (inputFile.is_open())
	{
		int totalNodes;

		std::string line;
		// Pula linhas de comentário se houver (começando com #)
		while (inputFile.peek() == '#' || inputFile.peek() == '\n') {
			std::getline(inputFile, line);
		}

		// 1. Number of nodes (including depot)
		inputFile >> totalNodes;
		nbClients = totalNodes - 1;

		// 2. Number of time intervals |H|
		inputFile >> nbTimeIntervals;

		// 3. Length of each time interval T
		inputFile >> intervalLength;

		// Inicializa vetores
		service_time.resize(totalNodes);
		demands.resize(totalNodes, 0.0); // TSP não tem demanda, zeramos
		x_coords.resize(totalNodes, 0.0); // TD-TSP sem coordenadas, zeramos
		y_coords.resize(totalNodes, 0.0);
		
		// 4. List of service time s at each node
		for (int i = 0; i < totalNodes; i++) {
			inputFile >> service_time[i];
		}

		// 5. Travel time matrices (sequentially for each interval h)
		// timeCostsTD[h][i][j]
		timeCostsTD.resize(nbTimeIntervals, 
			std::vector<std::vector<double>>(totalNodes, std::vector<double>(totalNodes)));

		for (int h = 0; h < nbTimeIntervals; h++) {
			for (int i = 0; i < totalNodes; i++) {
				for (int j = 0; j < totalNodes; j++) {
					inputFile >> timeCostsTD[h][i][j];
				}
			}
		}

		// Configurações Padrão
		durationLimit = 1.e30; 
		vehicleCapacity = 1.e30; 
		isDurationConstraint = false;
	}
	else
	{
		throw std::string("Impossible to open instance file: " + pathToInstance);
	}
}