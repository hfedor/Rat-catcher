#include "Sentence1.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <vector>

Sentence1::Sentence1(std::vector<std::string> constructorValues) // explicit constructor with given parameter list
{
	Sentence1::subject = constructorValues[0];
	Sentence1::action = constructorValues[1];
	Sentence1::attributive = constructorValues[2];
	Sentence1::negation = false;
}

std::string Sentence1::sentence1Generator()
{
	int randomizerOfNegation;
	
	srand(time(NULL)); // Initialize random seed
	randomizerOfNegation = rand() % 100 + 1; // random value from 1 to 100
	
	if(randomizerOfNegation % 2 == 0) // There is negation in the sentence 
		Sentence1::negation = true;
	
	else // There is negation in the sentence
		Sentence1::negation = false;
	
	return createSentence();
}

std::string Sentence1::createSentence()
{
	std::string dontWordsArray[] = {"I", "They", "Me", "You"}; // Words that go with "don't";
	std::vector<std::string> dontWords;
	dontWords.push_back("I");
	dontWords.push_back("They");
	dontWords.push_back("Me");
	dontWords.push_back("You");
	dontWords.push_back("i");
	dontWords.push_back("they");
	dontWords.push_back("me");
	dontWords.push_back("you");
	
	std::string resultSentence;
	
	for(int i = 0; i < dontWords.size(); i++)
	{
		if(Sentence1::subject == dontWords[i])
		{
			if(Sentence1::negation)
				return resultSentence = Sentence1::subject + " don't " + Sentence1::action + " " + Sentence1::attributive + ".";
			else
				return resultSentence = Sentence1::subject + " " + Sentence1::action + " " + Sentence1::attributive + ".";
		}
	}
	// If we exit this loop it's third person sentence
	if(Sentence1::negation)
		return resultSentence = Sentence1::subject + " doesn't " + Sentence1::action + " " + Sentence1::attributive + ".";
	else
		return resultSentence = Sentence1::subject + "s " + Sentence1::action + " " + Sentence1::attributive + ".";
}


