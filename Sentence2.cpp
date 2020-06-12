#include "Sentence2.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <vector>

Sentence2::Sentence2(std::vector<std::string> constructorValues) // explicit constructor with given parameter list
{
	Sentence2::subject = constructorValues[0];
	Sentence2::action = constructorValues[1];
	Sentence2::attributive = constructorValues[2];
	Sentence2::negation = false;
	Sentence2::subjectSecond = constructorValues[3];
	Sentence2::actionSecond = constructorValues[4];
	Sentence2::attributiveSecond = constructorValues[5];
	Sentence2::negationSecond = false;
}

std::string Sentence2::sentence2Generator()
{
	int randomizerOfFirstNegation;
	int randomizerOfSecondNegation;
	
	srand(time(NULL)); // Initialize random seed
	randomizerOfFirstNegation = rand() % 100 + 10; // random value 
	randomizerOfSecondNegation = rand() % 200 + 10; // random value 
	
	if(randomizerOfFirstNegation % 2 == 0) // There is negation in the sentence 
		Sentence2::negation = true;
	
	if(randomizerOfSecondNegation % 3 == 0) // There is negation in the second part of the sentence
		Sentence2::negationSecond = true;
	
	return createSentence();
}

std::string Sentence2::createSentence()
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
	
	std::string resultSentenceFirstPart;
	std::string resultSentenceSecondPart;
	
	for(int i = 0; i < dontWords.size(); i++)
	{
		if(Sentence2::subject == dontWords[i] && resultSentenceFirstPart.length() == 0)
		{
			if(Sentence2::negation)
				resultSentenceFirstPart = Sentence2::subject + " don't" + Sentence2::action + " " + Sentence2::attributive + ",";
			else
				resultSentenceFirstPart = Sentence2::subject + " " + Sentence2::action + " " + Sentence2::attributive + ",";
		}
		if(Sentence2::subjectSecond == dontWords[i] && resultSentenceSecondPart.length() == 0)
		{
			if(Sentence2::negationSecond)
				resultSentenceSecondPart = Sentence2::subjectSecond + " don't " + Sentence2::actionSecond + " " + Sentence2::attributiveSecond + ".";
			else
				resultSentenceSecondPart = Sentence2::subjectSecond + " " + Sentence2::actionSecond + " " + Sentence2::attributiveSecond + ".";
		}
	}
	
	if(Sentence2::negation && resultSentenceFirstPart.length() == 0)
		resultSentenceFirstPart = Sentence2::subject + " doesn't " + Sentence2::action + " " + Sentence2::attributive + ",";
	else if(!Sentence2::negation && resultSentenceFirstPart.length() == 0)
		resultSentenceFirstPart = Sentence2::subject + "s " + Sentence2::action + " " + Sentence2::attributive + ",";
	
	if(Sentence2::negationSecond && resultSentenceSecondPart.length() == 0)
		resultSentenceSecondPart = Sentence2::subjectSecond + " doesn't " + Sentence2::actionSecond + " " + Sentence2::attributiveSecond + ".";
	else if(!Sentence2::negationSecond && resultSentenceSecondPart.length() == 0)
		resultSentenceSecondPart = Sentence2::subjectSecond + "s " + Sentence2::actionSecond + " " + Sentence2::attributiveSecond + ".";

	return resultSentenceFirstPart + resultSentenceSecondPart;
}


