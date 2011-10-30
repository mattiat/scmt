//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef NEGATIVEAUTOREGULATION_H_
#define NEGATIVEAUTOREGULATION_H_

#include "Network.h"

/**
 * Reads in genes, edges and sets up signals and input genes
* @return 0 if successful
*/
int Network::readInput() { // TODO: this will be read from file
	std::cerr << "\nReading in network structure...";

	// ONE GENE NEGATIVE AUTOREGULATION
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


#endif /* NEGATIVEAUTOREGULATION_H_ */
