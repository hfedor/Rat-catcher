#include <iostream>
#include <list>

/*
	interface
*/
class SentenceGenerator
{
private:
	std::list<std::string> nouns;
	std::list<std::string> verbs;
	std::list<std::string> adjectives;
	std::list<std::string> conjunction;
	
	void uploadWords();	// load words from file/db 
public:
		
};