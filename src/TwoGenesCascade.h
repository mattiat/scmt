//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef TWOGENESCASCADE_H_
#define TWOGENESCASCADE_H_

#include "Network.h"

/**
 * Reads in genes, edges and sets up signals and input genes
* @return 0 if successful
*/
int Network::readInput() { // TODO: this will be read from file
	std::cerr << "\nReading in network structure...";

	// TWO GENES CASCADE
	// delay in the response of the second gene
	// reading in genes
	Gene* X = new Gene("X",AND,2.0);
	Gene* Y = new Gene("Y",OR,1.0);
	genes.push_back(X);
	genes.push_back(Y);
	// reading in edges
	Edge* Exy = new Edge("Exy",X,Y,20.0,14.0);
	X->addOutgoingEdge(Exy);
	Y->addIncomingEdge(Exy);
	edges.push_back(Exy);
	// setting up in signals and inputs
	Signal* Sx = new Signal("Sx",X,30.0);
	X->addIncomingEdge(Sx);
	inputGenes.push_back(X);
	signals.push_back(Sx);

	std::cerr << " done\n";
	return 0;
}


#endif /* TWOGENESCASCADE_H_ */
