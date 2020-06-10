#include <iostream>

#include "Sentence.h"

/*
	First type of sentence
*/
class Sentence1	:	public Sentence {
private:
	bool  negation;				// is there "no" in sentence
	std::string subject;		// main noun
	std::string action;			// activity of subiect
	std::string attributive; 	// feature of the subject
public:
};