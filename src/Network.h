//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#ifndef NETWORK_H_
#define NETWORK_H_
#include <vector>
#include "Edge.h"
#include "Gene.h"
#include "Signal.h"

/**
 * transcription network
 */
class Network {
public:
	Network(std::string id);
	virtual ~Network();
	static std::string welcome();
	static std::string goodbye(int retVal);
	int readInput();
	void draw();
	int runSimulations();
	int runApproximations();
	int calculateApproximations();
	void analyzeResults();
	std::string id;
private:
	std::vector<Gene*> genes;
	std::vector<Edge*> edges;
	std::vector<Gene*> inputGenes;
	std::vector<Signal*> signals;

	friend std::ostream& operator<<(std::ostream&, const Network&);
};

std::ostream& operator<<(std::ostream&, const Network&);

#endif /* NETWORK_H_ */
