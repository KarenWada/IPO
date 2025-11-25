#ifndef INSTANCECVRPLIB_H
#define INSTANCECVRPLIB_H
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

class InstanceCVRPLIB
{
public:
	// Dados Geográficos (Zerados se não fornecidos no TD-TSP)
	std::vector<double> x_coords;
	std::vector<double> y_coords;

	// Dados TD-TSP
	std::vector<double> service_time;
	int nbClients;                  // Número de clientes (excluindo depósito)
	int nbTimeIntervals;            // |H|
	double intervalLength;          // T
	
	// Matriz 3D: [Intervalo][Origem][Destino]
	std::vector<std::vector<std::vector<double>>> timeCostsTD; 

	// Constraints (Mantidos para compatibilidade, mas Capacity será ignorado)
	double durationLimit = 1.e30;
	double vehicleCapacity = 1.e30; 
	bool isDurationConstraint = false;

	// Construtor
	InstanceCVRPLIB(std::string pathToInstance);
};

#endif //INSTANCECVRPLIB_H