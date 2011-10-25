//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef NUMERICAL_H_
#define NUMERICAL_H_

class Numerical {
public:
	static double zero() {
		return epsilon;
	}
	static bool isZero(double d) {
		return (0 <= d && d <= epsilon);
	}
private:
	static const double epsilon = 1e-3;
};


#endif /* NUMERICAL_H_ */
