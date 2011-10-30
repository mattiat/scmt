//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef POSITIVEAUTOREGULATION_H_
#define POSITIVEAUTOREGULATION_H_

#include "Network.h"

/**
 * Reads in genes, edges and sets up signals and input genes
* @return 0 if successful
*/
int Network::readInput() { // TODO: this will be read from file
	std::cerr << "\nReading in network structure...";

	// ONE GENE POSITIVE AUTOREGULATION 
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

	std::cerr << " done\n";
	return 0;
}


#endif /* POSITIVEAUTOREGULATION_H_ */
