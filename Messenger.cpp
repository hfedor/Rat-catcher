#include "Messenger.h"

using namespace std;

std::string return_current_time_and_date()
{
	auto now = std::chrono::system_clock::now();
	auto in_time_t = std::chrono::system_clock::to_time_t(now);
	
	std::stringstream ss;
	ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
	return ss.str();
}

Messenger::Messenger(string messanger_name)
{
	name = messanger_name;
	generateMessage();
}

Messenger::Messenger(std::string messanger_name, int synchronize)
{
	synchornize_mode = synchronize;
	name = messanger_name;
	generateMessage();
}

void Messenger::generateMessage()
{
	DBAccess  dba("sentenceGenerator.db");
	
	std::random_device rd;
    std::default_random_engine e2(rd()) ;
    std::uniform_real_distribution<> dist1(1, 4);
    std::uniform_real_distribution<> dist2(1, 10);
	
	int sent_numb = floor(dist2(e2));
	string tmp;
	
	for(int i = 0; i < sent_numb; i++)
	{
		int sent_type = floor(dist1(e2));
		if( sent_type  == 1)
		{
			Sentence1 sent1({
				dba.RandomNounFromDB(),
				dba.RandomVerbFromDB(),
				dba.RandomAdjectiveFromDB(),
			});
			tmp = sent1.createSentence();
			if(message.length() + tmp.length() <= 500)
				message += tmp;
		}
		else if( sent_type  == 2)
		{
			Sentence2 sent2({
				dba.RandomNounFromDB(),
				dba.RandomVerbFromDB(),
				dba.RandomAdjectiveFromDB(),
				dba.RandomNounFromDB(),
				dba.RandomVerbFromDB(),
				dba.RandomAdjectiveFromDB()
			});
			tmp = sent2.createSentence();
			if(message.length() + tmp.length() <= 500)
				message += tmp;
		}
		else if( sent_type  == 3)
		{
			Sentence3 sent3({
				dba.RandomNounFromDB(),
				dba.RandomVerbFromDB(),
				dba.RandomAdjectiveFromDB(),
			});
			tmp = sent3.createSentence();
			if(message.length() + tmp.length() <= 500)
				message += tmp;
		}
	}
	
	generate_end = std::chrono::steady_clock::now();
}

bool Messenger::sendMessage()
{
	if(synchornize_mode == 1)
	{
		DBMessageAccess dbma("messages.db");
		
		dbma.AddMessageToDB(message, return_current_time_and_date());
	}
	else
	{
		std::fstream file;
		file.open( file_name, std::ios::out  | ios::app);
		if( file.good() != true )
		{
			cout << "Cant't open the file \"" << file_name  << "\"!" << std::endl;
			return false;
		}
		
		file << *this << endl;
	}
	
	return true;
}


bool Messenger::sendMessage(string file_name)
{
	std::fstream file;
	file.open( file_name, std::ios::out  | ios::app);
	if( file.good() != true )
	{
		cout << "Cant't open the file \"" << file_name  << "\"!" << std::endl;
		return false;
	}
	
	file << *this << endl;
	
	return true;
}

std::string Messenger::	printMessage()
{
	string result = "";
	result += "<message>\n";
	result += "sender:\n";
	result += name;
	result += "\n"; 
    result += message;
	result += "\n"; 
	result += "</message>\n";
    return result;
}
	
	
ostream & operator<< ( ostream &out, Messenger &messenger)
{
	out << "<message>\n";
	out << "sender:\n";
	out << messenger.name << "\n"; 
	out << "sended:\n";
	
    //std::time_t now_c = chrono::system_clock::to_time_t(messenger.send);
    //out << std::put_time(std::localtime(&now_c), "%F %T") << '\n';
    out << return_current_time_and_date() << "\n";
	//cout << time_point_cast<nanoseconds>(messenger.send) << endl;
	out << messenger.message << "\n";
	out << "</message>" << endl;
    return out;
}