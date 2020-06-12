#include "Sentence3.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <vector>

Sentence3::Sentence3(std::vector<std::string> constructorValues) // explicit constructor with given parameter list
{
	Sentence3::subject = constructorValues[0];
	Sentence3::action = constructorValues[1];
	Sentence3::attributive = constructorValues[2];
	Sentence3::negation = false;
}

std::string Sentence3::sentence3Generator()
{
	int randomizerOfNegation;
	
	srand(time(NULL)); // Initialize random seed
	randomizerOfNegation = rand() % 100 + 1; // random value from 1 to 100
	
	if(randomizerOfNegation % 2 == 0) // There is negation in the sentence 
		Sentence3::negation = true;
	
	else // There is negation in the sentence
		Sentence3::negation = false;
	
	return createSentence();
}

std::string Sentence3::createSentence()
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
		if(Sentence3::subject == dontWords[i])
		{
			if(Sentence3::negation)
				return resultSentence = "Don't " + Sentence3::subject + " " + Sentence3::action + " " + Sentence3::attributive + "?";
			else
				return resultSentence = "Do " + Sentence3::subject + " " + Sentence3::action + " " + Sentence3::attributive + "?";
		}
	}
	// If we exit this loop it's third person sentence
	if(Sentence3::negation)
		return resultSentence = "Doesn't " + Sentence3::subject + " " + Sentence3::action + " " + Sentence3::attributive + "?";
	else
		return resultSentence = "Does " +Sentence3::subject + " " + Sentence3::action + " " + Sentence3::attributive + "?";
}


