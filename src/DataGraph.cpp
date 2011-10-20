//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#include "DataGraph.h"
#include "Globals.h"
#include <boost/foreach.hpp>
#include <fstream>
#include <iomanip>
#include <boost/lexical_cast.hpp>
#include <math.h>

double DataGraph::max_x = 0.0;
double DataGraph::max_y = 0.0;

DataGraph::DataGraph() {
	;
}

DataGraph::~DataGraph() {
	;
}

void DataGraph::addSimulationData(double time, double concentration) {
	simulationData.push_back(Point(time,concentration));
	updateMax_x(time);
	updateMax_y(concentration);
	return;
}

void DataGraph::addApproxData(double time, double concentration) {
	approxData.push_back(Point(time,concentration));
	// time is not updated by approximation data
	updateMax_y(concentration);
	return;
}

const Point DataGraph::getApproxDataLIFO() {
	return approxData.back();
}

double DataGraph::getMax_x_int() {
	return max_x;
}

void DataGraph::updateMax_x(double n) {
	if(n > DataGraph::max_x) DataGraph::max_x = n;
}
void DataGraph::updateMax_y(double n) {
	if(n > DataGraph::max_y) DataGraph::max_y = n;
}

std::string DataGraph::getMax_x_str() {
	int boundary = ceil(DataGraph::max_x + DataGraph::max_x*20/100);
	return boost::lexical_cast<std::string>(boundary);
}

std::string DataGraph::getMax_y_str() {
	int boundary = ceil(DataGraph::max_y + DataGraph::max_y*20/100);
	return boost::lexical_cast<std::string>(boundary);
}

void DataGraph::setPrecision(int digits, std::ofstream& stream) {
	stream << std::fixed << std::setprecision(5);
}

void DataGraph::toFile(std::string geneId) {
	/** storing simulation data */
	dataFileSimulation = std::string(WRITABLE_DIR) + geneId + "_simulation.dat";
	std::ofstream stream;
	stream.open(dataFileSimulation.c_str());
	setPrecision(5, stream);
	BOOST_FOREACH(Point p, simulationData) {
		stream << p.first << "\t" << p.second << "\n";
	}
	stream.close();
	/** storing approximation data */
	dataFileApprox = std::string(WRITABLE_DIR) + geneId + "_approx.dat";
	stream.open(dataFileApprox.c_str());
	BOOST_FOREACH(Point p, approxData) {
		stream << p.first << "\t" << p.second << "\n";
	}
	stream.close();
	return;
}

void DataGraph::createPlotScript(std::string geneId) {
	plotScript = std::string(WRITABLE_DIR) + geneId + ".sh";
	graphImage = std::string(WRITABLE_DIR) + geneId + ".png";
	std::ofstream stream(plotScript.c_str());
	stream << "#!/usr/bin/gnuplot\n\n";
	stream << "set key right bottom\n";
	stream << "set ylabel \"X/X_st\"\n";
	stream << "set xlabel \"t/T_half\"\n";
	stream << "set xrange [0:" +getMax_x_str()+ "]\n";
	stream << "set yrange [0:" +getMax_y_str()+ "]\n";
	stream << "plot \"" + dataFileSimulation +
			"\" using 1:2 title \"Simulation\" with linespoints, \\\n";
	stream << "\"" + dataFileApprox +
			"\" using 1:2 title \"Euler approximation\" with linespoints \n";
	stream << "set term png\n";
	stream << "set output '" + graphImage + "'\n";
	stream << "replot";
	stream.close();
}

void DataGraph::plotGraph() {
	std::string allowX = "chmod +x " + plotScript;
	system(allowX.c_str());
	system(plotScript.c_str());
}
