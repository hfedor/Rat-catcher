#include "Messenger.h"

using namespace std;

Messenger::Messenger()
{
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
}

ostream & operator<< ( ostream &out, Messenger &messenger)
{
	out << "<message>\n";
	out << "sender:\n";
	out << messenger.name << "\n"; 
    out << messenger.message << "\n"; 
	out << "</message>" << endl;
    return out;
}