#include <iostream>
#include <list>
#include <fstream>

#include "DBAccess.h"

/*
	Cunsorship Program
*/
struct Forbiden{
	std::list<std::string> words;
};
class CensorshipProgram{
private:
	std::list<Forbiden> forbidens;
	std::string message = "";
	std::string censored = "";
public:
	CensorshipProgram();
	CensorshipProgram(int forbiden_numb);
	std::string addForbiden(int words_numb);
	std::string checkMessage();
	std::string loadMessage(std::string file_name);
	void generateForbidens();
	void generateForbidens(int forbiden_numb);
	void printForbidens();
};