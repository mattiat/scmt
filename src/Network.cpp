//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#include "Network.h"
#include "Globals.h"
#include <boost/foreach.hpp>
#include <iostream>

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
 * Reads in genes, edges and sets up signals and input genes
* @return 0 if successful
*/
int Network::readInput() { // TODO: this will be read from file
	std::cerr << "\nReading in network structure...";

//	// TWO GENES CASCADE
//	// delay in the response of the second gene
//	// reading in genes
//	Gene* X = new Gene("X",AND,2.0);
//	Gene* Y = new Gene("Y",OR,1.0);
//	genes.push_back(X);
//	genes.push_back(Y);
//	// reading in edges
//	Edge* Exy = new Edge("Exy",X,Y,20.0,14.0);
//	X->addOutgoingEdge(Exy);
//	Y->addIncomingEdge(Exy);
//	edges.push_back(Exy);
//	// setting up in signals and inputs
//	Signal* Sx = new Signal("Sx",X,30.0);
//	X->addIncomingEdge(Sx);
//	inputGenes.push_back(X);
//	signals.push_back(Sx);

	// ONE GENE AUTOREGULATION
	// POSITIVE
	Gene* X_pos = new Gene("X_pos",OR,2.0);
	// reading in genes
	genes.push_back(X_pos);
	// reading in edges
	Edge* Exx_pos = new Edge("Exx_pos",X_pos,X_pos,10.0,10.0);
	X_pos->addOutgoingEdge(Exx_pos);
	X_pos->addIncomingEdge(Exx_pos);
	// setting up in signals and inputs
	Signal* Sx_pos = new Signal("Sx_pos",X_pos,30.0);
	X_pos->addIncomingEdge(Sx_pos);
	inputGenes.push_back(X_pos);
	signals.push_back(Sx_pos);
	// NEGATIVE
	// _ to notice suppression effect, steady state with signal alone can't
	// be too close to autoregulation threshold
	// _ logic approximations of input functions create oscillations instead of
	// converging to a value in between the steady state with and the steady
	// state without autoregulation
	Gene* X_neg = new Gene("X_neg",OR,2.0);
	// reading in genes
	genes.push_back(X_neg);
	// reading in edges
	Edge* Exx_neg = new Edge("Exx_neg",X_neg,X_neg,-10.0,10.0);
	X_neg->addOutgoingEdge(Exx_neg);
	X_neg->addIncomingEdge(Exx_neg);
	// setting up in signals and inputs
	Signal* Sx_neg = new Signal("Sx_neg",X_neg,30.0);
	X_neg->addIncomingEdge(Sx_neg);
	inputGenes.push_back(X_neg);
	signals.push_back(Sx_neg);


	std::cerr << " done\n";
	return 0;
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
