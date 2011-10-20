//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#ifndef DATAGRAPH_H_
#define DATAGRAPH_H_
#include <string>
#include <vector>

typedef std::pair<double,double> Point;

/**
* collection of data that represent 2D functions
*/
class DataGraph {
public:
	DataGraph();
	~DataGraph();
	void addSimulationData(double time, double concentration);
	void addApproxData(double time, double concentration);
	const Point getApproxDataLIFO();
	static double getMax_x_int();
	void toFile(std::string geneId);
	void createPlotScript(std::string geneId);
	void plotGraph();
private:
	void setPrecision(int digits, std::ofstream&);
	void updateMax_x(double);
	void updateMax_y(double);
	std::string getMax_x_str();
	std::string getMax_y_str();
	std::vector<Point> simulationData;
	std::vector<Point> approxData;
	std::string dataFileSimulation;
	std::string dataFileApprox;
	std::string plotScript;
	std::string graphImage;
	static double max_x;
	static double max_y;
};

#endif
