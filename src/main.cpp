//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#include "Network.h"
#include "Gene.h"
#include "Edge.h"
#include <iostream>

int main(int argc, char** argv) {
	// TODO: consider encapsulating main() via a Caller object
	std::cerr << Network::welcome();
	int retVal(0);
  Network myNetwork("TEST");
  retVal += myNetwork.readInput();
  std::cerr << myNetwork;
  retVal += myNetwork.runSimulations();
  retVal += myNetwork.calculateApproximations();
  myNetwork.analyzeResults();
  std::cerr << Network::goodbye(retVal);
 	return retVal;
}
