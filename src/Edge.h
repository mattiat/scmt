//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#ifndef EDGE_H_
#define EDGE_H_
#include <string>
#include <iostream>

class Gene;
class Signal;

/**
 * edge in a gene transcription network
 */
class Edge {
public:
	Edge(std::string id, Gene* o, Gene* d, double s, double t);
	virtual ~Edge();
	const std::string getId();
private:
	std::string id;
	Gene* origin;
	Gene* destination;
	double strength;
	double threshold;
	const double* concentration;

	friend class Gene;
	friend class Signal;
	friend std::ostream& operator<<(std::ostream&, const Edge&);
	friend std::ostream& operator<<(std::ostream&, const Signal&);
};

std::ostream& operator<<(std::ostream&, const Edge&);

#endif /* EDGE_H_ */
