#ifndef INSTANCECVRPLIB_H
#define INSTANCECVRPLIB_H
#include <string>
#include <vector>

class InstanceCVRPLIB
{
public:
	// Dados Geográficos (Zerados se não fornecidos no TD-TSP)
	std::vector<double> x_coords;
	std::vector<double> y_coords;

	// Dados TD-TSP
	std::vector<double> service_time;
	std::vector<double> demands;    // Adicionado para compatibilidade com Params
	int nbClients;                  
	int nbTimeIntervals;            
	double intervalLength;          
	
	// Matriz 3D: [Intervalo][Origem][Destino]
	std::vector<std::vector<std::vector<double>>> timeCostsTD; 

	// Constraints
	double durationLimit = 1.e30;
	double vehicleCapacity = 1.e30; 
	bool isDurationConstraint = false;

	// Construtor Simplificado
	InstanceCVRPLIB(std::string pathToInstance);
};

#endif //INSTANCECVRPLIB_H