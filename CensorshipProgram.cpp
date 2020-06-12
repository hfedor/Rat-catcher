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

std::string CensorshipProgram::censureMessage()
{
	string s = "";
	for(int i = 0 ; i < message.length(); i ++)
	{
		if( (message[i] >= 65 && message[i] <= 90) || (message[i] >= 97 && message[i] <= 122) )
			s += message[i];
		else 
		{
			if(!checkWord(s))
				censored += s;
			s = "";
			censored += message[i];
		}
	}
}

bool  CensorshipProgram::checkWord(string word)
{
	for(Forbiden f : forbidens)
		for(string w : f.words)
			if(w == word)
				return true;
	return false;
}

std::string CensorshipProgram::loadMessage(std::string file_name)
{
	std::fstream file;
	string line;
	string upload;
	file.open( file_name, ios::in);
	if( file.good() != true )
	{
		cout << "Cant't open the file \"" << file_name  << "\"!" << std::endl;
		return "";
	}
	
	getline(file,line);
	if(line != "<message>")
	{
		file.close();
		cout << "Invalid message!" << endl;
		return "";
	}
	getline(file,line);
	if(line != "sender:")
	{
		file.close();
		cout << "Invalid message!" << endl;
		return "";
	}
	getline(file,line);
	while(getline(file,line))
	{
		if(line == "</message>")
		{
			message += line;
			
			std::ofstream uploaded_file;
			uploaded_file.open( file_name, ios::out );
			if( uploaded_file.good() != true )
			{
				cout << "Cant't open the file \"" << file_name  << "\"!" << std::endl;
				uploaded_file.close();
				return message;
			}
			
			while(getline(file,upload))
				if(upload == "<message>")
				{
					uploaded_file << upload << endl;
					break;
				}
			while(getline(file,upload))
					uploaded_file << upload << endl;
					
			uploaded_file.close();
			file.close();
			return message;
		}
		else if(line == "<message>")
		{
			cout << "Invalid message!" << endl;
			file.close();
			return "";
		}
		
		message += line;
	}
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

void CensorshipProgram::printMessage()
{
	cout << message << endl;
}

void CensorshipProgram::printCensored()
{
	cout << censored << endl;
}