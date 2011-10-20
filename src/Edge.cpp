//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#include "Edge.h"
#include "Gene.h"
#include <string>

Edge::Edge(std::string id, Gene* origin, Gene* destination, double strength,
		double threshold)
: id(id),
  origin(origin),
  destination(destination),
  strength(strength),
  threshold(threshold),
  concentration(&origin->concentration) {
	;
}

Edge::~Edge() {
	;
}

const std::string Edge::getId() {
	return id;
}

std::ostream& operator<<(std::ostream& s, const Edge& e) {
	s << "Edge " << e.id << std::endl;
	s << "  origin: " << e.origin->id << std::endl;
	s << "  destination: " << e.destination->id << std::endl;
	s << "  strength: " << e.strength << std::endl;
	s << "  threshold: " << e.threshold << std::endl;
	return s;
}
