//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#ifndef GENE_H_
#define GENE_H_
#include "DataGraph.h"
#include "InputType.h"
#include <vector>
#include <string>
#include <boost/thread.hpp>

class Edge;

/**
 * node in a gene transcription network
 */
class Gene { // TODO: consider creating Simulation and Approximation objects
public:
	Gene(std::string id, InputType iF, double dR);
	virtual ~Gene();
	const std::string getId();
	void addOutgoingEdge(Edge*);
	void addIncomingEdge(Edge*);
	int simulateTranslation(double time);
	static void joinSimulations(std::vector<Gene*>);
	int approximteTranslation();
	bool isRunning();
	int plotConcentrationGraph();
private:
	std::string id;
	std::vector<Edge*> incoming;
	std::vector<Edge*> outgoing;
	double time;
	InputType inputFunctionType;
	double degradationRate;
	double concentration;
	double previousConcentration;
	DataGraph concentrationGraph;
	boost::thread simulation;
	bool running;
	bool inputAboveThreshold();
	double getProductionRate();
	void run();
	bool simulationConverged();

	friend class Edge;
	friend std::ostream& operator<<(std::ostream&, const Edge&);
	friend std::ostream& operator<<(std::ostream&, const Gene&);
};

std::ostream& operator<<(std::ostream&, const Gene&);

#endif /* GENE_H_ */
