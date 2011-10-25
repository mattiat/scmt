//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#include "Signal.h"
#include "Gene.h"
#include <iostream>

/**
 * a signal is an edge with no origin and threshold < currentConcentration
 * @param destination gene that will be activated
 */
Signal::Signal(std::string id, Gene* destination, double strength):
Edge(id,NULL,destination,strength,0.0) {
	concentration = new double(1.0);
}

Signal::~Signal() {
	delete concentration;
}

std::ostream& operator<<(std::ostream& s, const Signal& sg) {
	s << "Signal " << sg.id << std::endl;
	s << "  destination: " << sg.destination->getId() << std::endl;
	s << "  strength: " << sg.strength << std::endl;
	return s;
}
