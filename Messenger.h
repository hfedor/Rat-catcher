#include <iostream>
#include <list>
#include <fstream>

#include "DBAccess.h"
#include "Sentence1.h"
#include "Sentence2.h"
#include "Sentence3.h"

/*
	Cunsorship Program
*/
class Messenger{
private:
	std::string name;
	std::string message = "";
	int char_limit = 500;
public:
	Messenger();
	void generateMessage();
	friend std::ostream & operator<< (std::ostream &out, Messenger &message);
};