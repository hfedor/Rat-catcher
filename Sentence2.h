#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Sentence.h"

/*
	second type of sentence - complexed
*/
class Sentence2	:	public Sentence {
private:
	bool  negation;		// is there "no" in sentence
	std::string subject;		// main noun
	std::string action;		// activity of subiect
	std::string attributive; 	// feature of the subject
	bool  negationSecond;			// is there "no" in second sentence
	std::string subjectSecond;		// second noun
	std::string actionSecond;		// second activity of subiect
	std::string attributiveSecond; 	// second feature of the subject
	
public:
	explicit Sentence2(std::vector<std::string> constructorValues); // explicit contructor
	std::string sentence2Generator();
	std::string createSentence();
};
