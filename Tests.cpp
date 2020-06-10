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
	
	int noun_id =  dba.AddNounToDB("noun1");
	cout << noun_id << endl;
	
	if( dba.GetNoun(noun_id) != "noun1")
		throw PreperExceptionData("DBATests", "GetNoun || AddNounToDB error:", ""); 
	
	dba.SetNoun(noun_id, "noun2");
	if( dba.GetNoun(noun_id) != "noun2")
		throw PreperExceptionData("DBATests", "GetNoun || SetNoun error:", ""); 
	
	 dba.GetNounsInfoFromDB("tasks_copies_info_test.txt");
	 dba.GetNounInfoFromDB(noun_id, "tasks_copies_info_test.txt");
	 dba.GetNounsInfoFromDB(cout);
	 dba.GetNounInfoFromDB(noun_id, cout);
	
	 dba.RemoveNounFromDB(noun_id);
		
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