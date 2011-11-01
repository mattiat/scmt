//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef AUTOREGULATION_H_
#define AUTOREGULATION_H_

#include "Network.h"

/**
 * Reads in genes, edges and sets up signals and input genes
* @return 0 if successful
*/
int Network::readInput() { // TODO: this will be read from file
	std::cerr << "\nReading in network structure...";

    positive = true;
	// ONE GENE AUTOREGULATION
	// _ to notice suppression effect, steady state with signal alone can't
	// be too close to autoregulation threshold
	Gene* X = new Gene("X",OR,2.0);
	// reading in genes
	genes.push_back(X);
	// reading in edges
    if(positive) Edge* Exx = new Edge("Exx",X,X,10.0,10.0);
    else Edge* Exx = new Edge("Exx",X,X,-10.0,10.0);
	X->addOutgoingEdge(Exx);
	X->addIncomingEdge(Exx);
	// setting up in signals and inputs
	Signal* Sx = new Signal("Sx",X,30.0);
	X->addIncomingEdge(Sx);
	inputGenes.push_back(X);
	signals.push_back(Sx);

	std::cerr << " done\n";
	return 0;
}


#endif /* AUTOREGULATION_H_ */
