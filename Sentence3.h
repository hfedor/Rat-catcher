#pragma once
#include <iostream>
#include <vector>
#include "Sentence.h"

/*
	Second type of sentence - question
*/
class Sentence3	:	public Sentence {
private:
	bool  negation;		// is there "no" in sentence
	std::string subject;		// main noun
	std::string action;		// activity of subiect
	std::string attributive; 	// feature of the subject
public:
	std::string sentence3Generator();
	explicit Sentence3(std::vector<std::string> constructorValues); // explicit contructor
	std::string createSentence(); // Support method to create sentence
};
