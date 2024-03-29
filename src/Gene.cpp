//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#include "Gene.h"
#include "Edge.h"
#include "Globals.h"
#include "Numerical.h"
#include <iostream>
#include <math.h>
#include <boost/foreach.hpp>
#include <fstream>

Gene::Gene(std::string id, InputType inputFunctionType, double degradationRate)
: id(id),
  inputFunctionType(inputFunctionType),
  degradationRate(degradationRate),
  concentration(0.0),
  previousConcentration(-1.0),
  absoluteTime(0.0),
  tauTime(-1.0) {
	;
}

Gene::~Gene() {
	;
}

const std::string Gene::getId() {
	return id;
}

bool Gene::inputAboveThreshold() {
	switch(inputFunctionType) {
	case AND: {
		BOOST_FOREACH(Edge* i, incoming) {
			if(*(i->concentration) < i->threshold) return false;
		}
		return true;
	}
	case OR: {
		BOOST_FOREACH(Edge* i, incoming) {
			if(*(i->concentration) > i->threshold) return true;
		}
		return false;
	}
	}
}

/**
 * production rate is the sum of the strengths of the incoming edges that are
 * above their respective input thresholds
 */
double Gene::getProductionRate() {
	double productionRate(0.0);
	BOOST_FOREACH(Edge* i, incoming) {
		if(*(i->concentration) > i->threshold)
			productionRate+=i->strength;
	}
	return productionRate;
}

std::ostream& operator<<(std::ostream& s, const Gene& g) {
	s << "Gene " << g.id << std::endl;
	s << "  input type: " << g.inputFunctionType << std::endl;
	s << "  degradation rate: " << g.degradationRate << std::endl;
	return s;
}

void Gene::addOutgoingEdge(Edge* e) {
	outgoing.push_back(e);
}

void Gene::addIncomingEdge(Edge* e) {
	incoming.push_back(e);
}

/**
 * a simulation is considered to have converged when the concentration of
 * proteins produced is sufficiently close to zero or when more than
 * MAX_TIME has elapsed
 */
bool Gene::simulationConverged() {
	double concentrationChange = concentration-previousConcentration;
	if(Numerical::isZero(concentrationChange) ||
			absoluteTime >= MAX_TIME)
		return true;
	else
		return false;
}

bool Gene::isRunning() { // TODO: make this atomic
 return running;
}

/**
* It is run whenever the activation factor(s) binding to the gene reach the
* active concentration (isInputAboveThreshold()==true) and starts translation
* @return 0 if successful
*/
int Gene::simulateTranslation(double time) { // TODO: make this atomic
	std::cerr << "Activating Gene " + id + "...\n";
	/** time at which the gene becomes activated */
	absoluteTime = time;
	tauTime = absoluteTime;
	concentrationGraph.addSimulationData(absoluteTime,concentration);
	concentrationGraph.addApproxData(absoluteTime,concentration);
	/** starts simulation */
	running = true;
	simulation = boost::thread(&Gene::run, this);
	return 0;
}

/**
 * barrier: wait for simulations associated with each gene to have terminated
 * before returning.
 */
void Gene::joinSimulations(std::vector<Gene*> genes) {
	BOOST_FOREACH(Gene* i, genes) {
		i->simulation.join();
	}
}

/**
* simulates the production of the associated protein
*/
void Gene::run() {
	bool above = true;
	while(! simulationConverged()) {
		absoluteTime+=TIME_STEP; // TODO: consider making staff atomic in Gene::run()
		double localTime = absoluteTime-tauTime;
		/** update current concentration */
		double steadyState = getProductionRate()/degradationRate;
		previousConcentration = concentration;
		if(inputAboveThreshold()) {
			/** just crossed the threshold? reset tau **/
			if (above == false) { above = true; tauTime=absoluteTime; }
			localTime = absoluteTime-tauTime;
			/** gene is active, protein is being produced */
			concentration = steadyState*(1-(exp(-degradationRate*localTime)));
		}
		else {
			/** just crossed the threshold? reset tau **/
			if (above == true) {above = false; tauTime=absoluteTime; }
			localTime = absoluteTime-tauTime;
			/** gene is inactive, protein is being dissipated */
			concentration = steadyState*(exp(-degradationRate*localTime));
		}
		concentrationGraph.addSimulationData(absoluteTime, concentration);
		/** verify whether connected genes are ready to become active */
		BOOST_FOREACH(Edge* o, outgoing) {
			if(o->destination->isRunning() == false
					&& o->destination->inputAboveThreshold()) {
				o->destination->simulateTranslation(absoluteTime);
			}
		}
	}
	return;
}

/**
 * uses a numerical method to approximate the results of the differential
 * equations in the model assuming all inputs to the nodes are active (i.e
 * maximal, constant production rates for each node)
 * @return 0 if successful
 */
int Gene::approximteTranslation() {
	// TODO: this is cheating, cannot use activation times based on protein
	// concentration from simulation in the approximation procedure. Must
	// recalculate activation times. Use threads (?)
	double xPrevious = concentrationGraph.getApproxData().first;
	double yPrevious = concentrationGraph.getApproxData().second;
	while (xPrevious < DataGraph::getMax_x_int()) {
		/** calculate slope */
		double derivate = getProductionRate()-degradationRate*yPrevious;
		/** calculate nex point */
		double x = xPrevious + TIME_STEP;
		double y = yPrevious + (derivate * TIME_STEP);
		concentrationGraph.addApproxData(x,y);
		/** update variables */
		xPrevious = x;
		yPrevious = y;
	}
	return 0;
}

int Gene::plotConcentrationGraph() {
	/** store data in .dat files */
	concentrationGraph.toFile(id);
	/** create gnuplot script */
	concentrationGraph.createPlotScript(id);
	/** run gnuplot */
	concentrationGraph.plotGraph();
	return 0;
}
