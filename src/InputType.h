//============================================================================
// Name        : TranscriptionNetwork
// Author      : Mattia Tomasoni
// Institution : University of Leeds
//============================================================================

#ifndef INPUTTYPE_H_
#define INPUTTYPE_H_

/**
 * represents the possible ways in which multiple values in input to a gene can
 * be combined
 */
enum InputType {AND, OR};
/*inline std::ostream& operator<< (std::ostream& os, const InputType& i) {
	switch (i) { // TODO operator<< for InputType append a string
	case 0: return os << "AND";
	case 1: return os <<"OR";
	}
}*/


#endif /* INPUTTYPE_H_ */
