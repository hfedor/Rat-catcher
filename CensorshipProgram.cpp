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
	
	DBAchivesAccess dbaa("achives.db");
	
	//dbaa.FindMessage(message,sended);
	
	message = "";
	
	return censored;
}

bool  CensorshipProgram::checkWord(string word)
{
	for(Forbiden f : forbidens)
		for(string w : f.words)
			if(w == word)
				return true;
	return false;
}

std::string CensorshipProgram::loadMessage()
{
	message = "";
	DBMessageAccess dbma("messages.db");
	
	vector<int> mess_id = dbma.GetMessagesIndexes();
	
	int  id = mess_id.front();
	
	message = dbma.GetMessage(id);
	sended = dbma.GetMessageSended(id);
	
	dbma.RemoveMessageFromDB(id);
	
	loaded	= return_current_time_and_date();
	
	DBAchivesAccess dbaa("achives.db");
	
	id = dbaa.FindMessage(message, sended);
	
	dbaa.SetCensored(id, censored);
	dbaa.SetLoaded(id, loaded);
	
	return message;
}

std::string CensorshipProgram::loadMessage(std::string file_name)
{
	message = "";
	std::fstream file;
	string line = "";
	string upload;
	file.open( file_name, ios::in);
	if( file.good() != true )
	{
		cout << "Cant't open the file \"" << file_name  << "\"!" << std::endl;
		return "";
	}
	
	file.seekg(0, ios::end);  
	if (file.tellg() == 0) {    
		cout << "Empty! file" << endl;
		return "";
	}
	
	while(line == "")
		getline(file,line);
	
	if(line != "<message>")
	{
		file.close();
		cout << "Invalid message1!" << line << " != <message>" << endl;
		return "";
	}
	getline(file,line);
	if(line != "sender:")
	{
		file.close();
		cout << "Invalid message2!" << endl;
		return "";
	}
	getline(file,line);
	getline(file,line);
	if(line != "sended:")
	{
		file.close();
		cout << "Invalid message3!" << endl;
		return "";
	}
	
	getline(file,line);
	sended = line;
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
			loaded	= return_current_time_and_date();
			
			DBAchivesAccess dbaa("achives.db");
	
			int id = dbaa.FindMessage(message, sended);
			
			dbaa.SetCensored(id, censored);
			dbaa.SetLoaded(id, loaded);
			
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