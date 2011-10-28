//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#include "Network.h"
#include "Globals.h"
#include <boost/foreach.hpp>
#include <iostream>

#include "TwoGenesCascade.h"
//#include "Autoregulation.h"

Network::Network(std::string id)
: id(id) {
	std::vector<Gene*> genes();
	std::vector<Edge*> edges();
	std::vector<Signal*> signals();
	std::vector<Gene*> inputGenes();
}

Network::~Network() {
	BOOST_FOREACH(Gene* g, genes) delete g;
	BOOST_FOREACH(Edge* e, edges) delete e;
	BOOST_FOREACH(Signal* s, signals) delete s;
}

std::string Network::welcome() {
	std::ostringstream oss;
	char* HOST = getenv("HOSTNAME");
	oss << "TRANSCRIPTION NETWORK version "
			<< VERION_MAJOR << "." << VERION_MINOR
			<< "\nwritten by Mattia Tomasoni (scmt@leeds.ac.uk)"
			<< "\nin the School of Computing at the University of Leeds."
			<< "\nRunning on " << HOST << std::endl;
	return oss.str();
}

std::string Network::goodbye(int retVal) {
	std::ostringstream oss;
	if (retVal == 0)
		oss << "\n\nTermination successful.\n";
	else
		oss << "\nErrors were encountered during execution.\n";
	return oss.str();
}

/**
* outputs a graphical representation of the network
*/
void Network::draw() {
	std::cerr << "\nDrawing network...\n";
	// TODO implement Network::draw()
	std::cerr << " done\n";
	return;
}

std::ostream& operator<<(std::ostream& s, const Network& n) {
	s << "\nPrinting network structure...\n";
	BOOST_FOREACH(Gene* g, n.genes) s << *g;
	BOOST_FOREACH(Edge* e, n.edges) s << *e;
	BOOST_FOREACH(Signal* sg, n.signals) s << *sg;
	return s;
}

/**
* run numerical simulation
* @return 0 if successful
*/
int Network::runSimulations() {
	int retVal(0);
	std::cerr << "\nStarting simulations...\n";
	double time = 0.0;
	/** activate input signals */
	// TODO: make this atomic with joinSimulations below
	BOOST_FOREACH(Gene* i, inputGenes) {
		retVal += i->simulateTranslation(time);
	}
	/** complete simulation for all genes before continuing */
	Gene::joinSimulations(genes);
	std::cerr << "...simulations completed.\n";
	return retVal;
}

/**
* run (Euler) approximations
* @return 0 if successful
*/
int Network::calculateApproximations() {
	int retVal(0);
	std::cerr << "\nCalculating approximations...\n";
	/** calculate approximations for all genes */
	BOOST_FOREACH(Gene* i, genes) {
		retVal += i->approximteTranslation();
	}
	std::cerr << "...approximations calculated.\n";
	return retVal;
}

void Network::analyzeResults() {
	BOOST_FOREACH(Gene* i, genes) {
		std::cerr << "\nAnalysing results for gene " << i->getId() << "...";
		i->plotConcentrationGraph();
		std::cerr << " done";
	}
}
