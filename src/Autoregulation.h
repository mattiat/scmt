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


#endif /* AUTOREGULATION_H_ */
