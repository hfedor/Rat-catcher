#include "Tests.h"

using namespace std;

bool Tests::testAll()
{
    try
	{
		DBATests();
		SentencesTests();
	}
	catch(exceptionData exc)
	{
		cout << exc << endl;
		return false;
	}
	cout << "All ok!" << endl;
	return true;
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
	
	cout << sent1.createSentence() << endl;
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