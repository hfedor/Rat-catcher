#include <iostream>
#include <vector>
#include "Sentence.h"

/*
	First type of sentence
*/
class Sentence1	:	public Sentence {
private:
	bool  negation;		// is there "no" in sentence
	std::string subject;		// main noun
	std::string action;		// activity of subiect
	std::string attributive; 	// feature of the subject
public:
	std::string sentence1Generator();
	explicit Sentence1(std::vector<std::string> constructorValues); // explicit contructor
	std::string createSentence(); // Support method to create sentence
};
