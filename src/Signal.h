//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Version     : 0.1
// Copyright   : University of Leeds
//============================================================================

#ifndef SIGNAL_H_
#define SIGNAL_H_

#include "Edge.h"

class Gene;

class Signal: public Edge {
public:
	Signal(std::string id, Gene* d, double s);
	virtual ~Signal();
	friend std::ostream& operator<<(std::ostream&, const Signal&);
};

std::ostream& operator<<(std::ostream&, const Signal&);

#endif /* SIGNAL_H_ */
