#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <chrono>

#include "DBAccess.h"
#include "DBMessageAccess.h"
#include "DBAchivesAccess.h"

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
	std::string loaded = "";
public:
	CensorshipProgram();
	CensorshipProgram(int forbiden_numb);
	std::string addForbiden(int words_numb);
	std::string censureMessage();
	bool checkWord(std::string);
	std::string loadMessage();
	std::string loadMessage(std::string file_name);
	void generateForbidens();
	void generateForbidens(int forbiden_numb);
	void printForbidens();
	void printMessage();
	void printCensored();
};

std::string return_current_time_and_date();