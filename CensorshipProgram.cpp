#include "CensorshipProgram.h"

using namespace std;

CensorshipProgram::CensorshipProgram()
{
	generateForbidens();
}

CensorshipProgram::CensorshipProgram(int forbiden_numb)
{
	generateForbidens(forbiden_numb);
}

std::string CensorshipProgram::addForbiden(int words_numb)
{
	DBAccess  dba("sentenceGenerator.db");
	Forbiden new_forbiden;
	string result = "";
	
	for(int i = 0; i < words_numb; i++)
		new_forbiden.words.push_back(dba.RandomNounFromDB());
	
	forbidens.push_back(new_forbiden);
	
	for( string s : new_forbiden.words){
		result += s;
		result += " ";
	}
	
	return result;	
}

std::string CensorshipProgram::checkMessage()
{
}

std::string CensorshipProgram::loadMessage(std::string file_name)
{
	
}

void CensorshipProgram::generateForbidens()
{
	std::random_device rd;
    std::default_random_engine e2(rd()) ;
    std::uniform_real_distribution<> dist1(1, 6);
    std::uniform_real_distribution<> dist2(1, 4);
	
	int forbiden_numb = floor(dist1(e2));
	
	for(int i = 0; i < forbiden_numb; i++)
		addForbiden(floor(dist2(e2)));
}

void CensorshipProgram::generateForbidens(int forbiden_numb)
{
	std::random_device rd;
    std::default_random_engine e2(rd()) ;
    std::uniform_real_distribution<> dist2(1, 4);
	
	for(int i = 0; i < forbiden_numb; i++)
		addForbiden(floor(dist2(e2)));
}

void CensorshipProgram::printForbidens()
{
	for(Forbiden f : forbidens)
	{
		cout << "{ ";
		for(string s : f.words)
		{
			cout << s << ", ";
		}
		cout << "}" << endl;
	}
}