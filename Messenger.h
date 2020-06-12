#pragma once
#include <iostream>
#include <list>
#include <fstream>
#include <chrono>

#include <ctime>   // localtime
#include <sstream> // stringstream
#include <iomanip> // put_time
#include <string>  // string

#include "DBAccess.h"
#include "DBMessageAccess.h"
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
	std::string file_name = "messeges.txt";	// name of text file where messenger sends messages
	std::chrono::steady_clock::time_point generate_start = std::chrono::steady_clock::now();
	std::chrono::steady_clock::time_point generate_end;
	std::chrono::steady_clock::time_point send;
public:
	Messenger(std::string messanger_name);
	void generateMessage();
	bool sendMessage(); 
	bool sendMessage(std::string file_name); 
	std::string printMessage();
	friend std::ostream & operator<< (std::ostream &out, Messenger &message);
};

