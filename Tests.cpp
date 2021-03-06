#include "Tests.h"

using namespace std;

bool Tests::testAll()
{
	
	synchroDBTests();
}

void Tests::initializeMutex()
{
	sem_init(Tests::mutex, 1, 1); // First argument is a pointer to our mutex, second one is wether its between processes or threads, third one is init value
}

bool Tests::archiveTests()
{
	std::cout << std::endl << "I am archive " << std::endl;
}

bool Tests::DBATests()
{
	DBAccess  dba("db_test.db");
	
	 dba.ClearNounsTable();
	 dba.ClearVerbsTable();
	 dba.ClearAdjectivesTable();
	
	int noun_id =  dba.AddNounToDB("noun1", 12);
	cout << noun_id << endl;
	
	if( dba.GetNoun(noun_id) != "noun1")
		throw PreperExceptionData("DBATests", "GetNoun || AddNounToDB error:", ""); 
	if( dba.GetNounFrequency(noun_id) != 12)
		throw PreperExceptionData("DBATests", "GetFrequency || AddNounToDB error:", ""); 
	
	dba.SetNoun(noun_id, "noun2");
	if( dba.GetNoun(noun_id) != "noun2")
		throw PreperExceptionData("DBATests", "GetNoun || SetNoun error:", ""); 
	dba.SetNounFrequency(noun_id, 13);
	if( dba.GetNounFrequency(noun_id) != 13)
		throw PreperExceptionData("DBATests", "GetNoun || SetNoun error:", ""); 
	
	 dba.GetNounsInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetNounInfoFromDB(noun_id, "tasks_copies_info_test.txt");
	 dba.GetNounsInfoFromDB(cout);
	 dba.GetNounInfoFromDB(noun_id, cout);
	 
	cout << dba.RandomNounFromDB() << endl;
	
	 dba.RemoveNounFromDB(noun_id);
	 
	int verb_id =  dba.AddVerbToDB("verb1",22);
	cout << verb_id << endl;
	
	if( dba.GetVerb(verb_id) != "verb1")
		throw PreperExceptionData("DBATests", "GetVerb || AddVerbToDB error:", ""); 
	
	dba.SetVerb(verb_id, "verb2");
	if( dba.GetVerb(verb_id) != "verb2")
		throw PreperExceptionData("DBATests", "GetVerb || SetVerb error:", ""); 
	
	 dba.GetVerbsInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetVerbInfoFromDB(verb_id, "tasks_copies_info_test.txt");
	 dba.GetVerbsInfoFromDB(cout);
	 dba.GetVerbInfoFromDB(verb_id, cout);
	
	 dba.RemoveVerbFromDB(verb_id);
	 
	int adjective_id =  dba.AddAdjectiveToDB("adjective1", 12);
	cout << adjective_id << endl;
	
	if( dba.GetAdjective(adjective_id) != "adjective1")
		throw PreperExceptionData("DBATests", "GetAdjective || AddAdjectiveToDB error:", ""); 
	if( dba.GetAdjectiveFrequency(adjective_id) != 12)
		throw PreperExceptionData("DBATests", "GetFrequency || AddAdjectiveToDB error:", ""); 
	
	dba.SetAdjective(adjective_id, "adjective2");
	if( dba.GetAdjective(adjective_id) != "adjective2")
		throw PreperExceptionData("DBATests", "GetAdjective || SetAdjective error:", ""); 
	dba.SetAdjectiveFrequency(adjective_id, 13);
	if( dba.GetAdjectiveFrequency(adjective_id) != 13)
		throw PreperExceptionData("DBATests", "GetAdjective || SetAdjective error:", ""); 
	
	 dba.GetAdjectivesInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetAdjectiveInfoFromDB(adjective_id, "tasks_copies_info_test.txt");
	 dba.GetAdjectivesInfoFromDB(cout);
	 dba.GetAdjectiveInfoFromDB(adjective_id, cout);
	
	 dba.RemoveAdjectiveFromDB(adjective_id);
		
	int conjunctive_id =  dba.AddConjunctiveToDB("conjunctive1", 12);
	cout << conjunctive_id << endl;
	
	if( dba.GetConjunctive(conjunctive_id) != "conjunctive1")
		throw PreperExceptionData("DBATests", "GetConjunctive || AddConjunctiveToDB error:", ""); 
	if( dba.GetConjunctiveFrequency(conjunctive_id) != 12)
		throw PreperExceptionData("DBATests", "GetFrequency || AddConjunctiveToDB error:", ""); 
	
	dba.SetConjunctive(conjunctive_id, "conjunctive2");
	if( dba.GetConjunctive(conjunctive_id) != "conjunctive2")
		throw PreperExceptionData("DBATests", "GetConjunctive || SetConjunctive error:", ""); 
	dba.SetConjunctiveFrequency(conjunctive_id, 13);
	if( dba.GetConjunctiveFrequency(conjunctive_id) != 13)
		throw PreperExceptionData("DBATests", "GetConjunctive || SetConjunctive error:", ""); 
	
	 dba.GetConjunctivesInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetConjunctiveInfoFromDB(conjunctive_id, "tasks_copies_info_test.txt");
	 dba.GetConjunctivesInfoFromDB(cout);
	 dba.GetConjunctiveInfoFromDB(conjunctive_id, cout);
	
	 dba.RemoveConjunctiveFromDB(conjunctive_id);
	 
	 
	DBAccess  generator("sentenceGenerator.db");
	
	cout << generator.RandomNounFromDB() << endl;
	cout << generator.RandomVerbFromDB() << endl;
	cout << generator.RandomAdjectiveFromDB() << endl;
	cout << generator.RandomConjunctiveFromDB() << endl;
	
	return true;
}

bool Tests::SentencesTests()
{
	DBAccess  dba("sentenceGenerator.db");
	
	Sentence1 sent1({
		dba.RandomNounFromDB(),
		dba.RandomVerbFromDB(),
		dba.RandomAdjectiveFromDB()
	});
	
	cout << "sent1: " << sent1.createSentence() << endl;
	
	Sentence2 sent2({
		dba.RandomNounFromDB(),
		dba.RandomVerbFromDB(),
		dba.RandomAdjectiveFromDB(),
		dba.RandomNounFromDB(),
		dba.RandomVerbFromDB(),
		dba.RandomAdjectiveFromDB()
	});
	
	cout << "sent2: " << sent2.createSentence() << endl;
	
	Sentence3 sent3({
		dba.RandomNounFromDB(),
		dba.RandomVerbFromDB(),
		dba.RandomAdjectiveFromDB()
	});
	
	cout << "sent3: " << sent3.createSentence() << endl;
}

bool Tests::CensorshipTests()
{
	std::cout << std::endl << "I am consumer" << std::endl;
	
	CensorshipProgram cp;
	/*
	cp.generateForbidens(10);
	
	cp.printForbidens();
	
	sem_wait(Tests::mutex); // We eneter the section
	
	cp.loadMessage("messages_test.txt");
	
	sem_post(Tests::mutex); // We leave the section
	
	cp.printMessage();
	
	cp.censureMessage();
	cout << "censored:" << endl;
	cp.printCensored();
	*/
	cp.loadMessage();
	
	cp.printMessage();
	
	cp.censureMessage();
	cout << "censored:" << endl;
	cp.printCensored();
}

bool Tests::MessengerTests()
{
	std::cout << std::endl << "I am producer" << std::endl;
	
	Messenger messenger("Kamilek");
	
	messenger.generateMessage();
	
	cout << messenger << endl;
	
	sem_wait(Tests::mutex); // We eneter the section
	
	messenger.sendMessage("messages_test.txt"	);
	
	sem_post(Tests::mutex); // We leave the section
	
	Messenger messenger2("Krzys");
	
	messenger2.generateMessage();
	
	cout << messenger2 << endl;
	
	sem_wait(mutex); // We eneter the section
	
	messenger2.sendMessage("messages_test.txt"	);
	
	sem_post(mutex); // We leave the section
	
	
	
	Messenger messenger3("Hubik");
	
	messenger3.generateMessage();
	
	cout << messenger3 << endl;
	
	messenger3.sendMessage();	
}

ostream & operator<< (ostream &out, const exceptionData &exc)
{
    return out << "Error in " << exc.thrownClass << "::" << exc.thrownFunction << ": " << exc.thrownStatement;
}

exceptionData PreperExceptionData(string className, string functionName, string statement)
{
    exceptionData exc;
    exc.thrownClass = className;
    exc.thrownFunction = functionName;
    exc.thrownStatement = statement;
    return exc;
}

bool Tests::synchroDBTests()
{
	CensorshipProgram cp;
	
	cp.generateForbidens(10);
	
	cp.printForbidens();
	
	Messenger messenger("Producer");
	
	for(int i = 0; i < 10000; i++)
	{
		std::cout << std::endl << "I am producer" << std::endl;
		
		messenger.generateMessage();
		
		messenger.sendMessage();
		
		cout << messenger << endl;
		
		std::cout << std::endl << "I am consumer" << std::endl;
		
		cp.loadMessage();
		
		cp.censureMessage();
		cout << "censored:" << endl;
		cp.printCensored();
	}
}

bool Tests::synchroTests()
{
	initializeMutex();
    	try
	{
		pid_t process_id_1;
		pid_t process_id_2;
		
		process_id_1 = fork(); // double fork
		process_id_2 = fork();
		
		if(process_id_1 == 0)
		{
			if(process_id_2 == 0)
				MessengerTests(); // producer
			else if(process_id_2 > 0)
				Tests::archiveTests(); // archive client
		}
			
		else if(process_id_1 > 0)
		{
			if(process_id_2 == 0)
				CensorshipTests(); // client
		}
		
		else if(process_id_1 < 0)
			std::cout << "error" << std::endl;
		else if(process_id_2 < 0)
			std::cout << "error" << std::endl;
		//DBATests();
		//SentencesTests();
		
		
	}
	catch(exceptionData exc)
	{
		cout << exc << endl;
		return false;
	}
	cout << "All ok!" << endl;
	return true;
}