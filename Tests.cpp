#include "Tests.h"

using namespace std;

bool Tests::testAll()
{
    try
	{
		DBATests();
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
	
	int noun_id =  dba.AddVerbToDB("noun1");
	cout << noun_id << endl;
	
	if( dba.GetVerb(noun_id) != "noun1")
		throw PreperExceptionData("DBATests", "GetVerb || AddVerbToDB error:", ""); 
	
	dba.SetVerb(noun_id, "noun2");
	if( dba.GetVerb(noun_id) != "noun2")
		throw PreperExceptionData("DBATests", "GetVerb || SetVerb error:", ""); 
	
	 dba.GetVerbsInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetVerbInfoFromDB(noun_id, "tasks_copies_info_test.txt");
	 dba.GetVerbsInfoFromDB(cout);
	 dba.GetVerbInfoFromDB(noun_id, cout);
	
	 dba.RemoveVerbFromDB(noun_id);
	 
	int verb_id =  dba.AddVerbToDB("verb1");
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
	 
	 int adjective_id =  dba.AddAdjectiveToDB("adjective1");
	cout << adjective_id << endl;
	
	if( dba.GetAdjective(adjective_id) != "adjective1")
		throw PreperExceptionData("DBATests", "GetAdjective || AddAdjectiveToDB error:", ""); 
	
	dba.SetAdjective(adjective_id, "adjective2");
	if( dba.GetAdjective(adjective_id) != "adjective2")
		throw PreperExceptionData("DBATests", "GetAdjective || SetAdjective error:", ""); 
	
	 dba.GetAdjectivesInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetAdjectiveInfoFromDB(adjective_id, "tasks_copies_info_test.txt");
	 dba.GetAdjectivesInfoFromDB(cout);
	 dba.GetAdjectiveInfoFromDB(adjective_id, cout);
	
	 dba.RemoveAdjectiveFromDB(adjective_id);
		
	return true;
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