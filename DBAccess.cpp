#include "DBAccess.h"

using namespace std;

// NODES:
	// CONSTRUCTOR:
DBAccess::DBAccess(string db_file_name)
{
	int rc = sqlite3_open(db_file_name.c_str(), &db);
	
	if( rc )
	{
		string message = "Can't open database: ";
		message += sqlite3_errmsg(db);
		cout << "DBAccess: " + message << endl;
	}
	else
	{
		BuildNounsTable();
		BuildVerbsTable();
		BuildAdjectivesTable();
		BuildConjunctivesTable();
	}
}

// TASKS INFO COPIES:
	// ADD TASKS:
int  DBAccess::AddNounToDB(string noun, int frequency)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM nouns WHERE noun = ? AND frequency = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,noun.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddNounToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO nouns (noun, frequency) VALUES (?,?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,noun.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddNounToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT id FROM nouns WHERE noun = \'" + noun + "\' AND frequency = " + to_string(frequency) + " ;";
	
	int newId;
	
	rc = sqlite3_exec(db, sql.c_str(), GetIdFromDB,&newId, &zErrMsg);
	
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL GetIdFromDB error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	} 
	else
			return newId;
		
   return -1;
}	

bool	DBAccess::BuildNounsTable()
{	
	int rc; // This line
	string sql; // This line
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists \"nouns\" ( \"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, \"noun\" TEXT NOT NULL,\"frequency\"	INTEGER NOT NULL DEFAULT 0);";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildNounsTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAccess::ClearNounsTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM nouns";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearNounsTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearNounsTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAccess::GetNoun(int id)
{
	return GetNounsRecord(id,1);
}

int DBAccess::GetNounFrequency(int id)
{
	return stoi(GetNounsRecord(id,2));
}

string DBAccess::GetNounsRecord(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM nouns WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, column);
		if(tmp == NULL)
			result = "0";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "GetNounsRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

bool DBAccess::GetNounsInfoFromDB(std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetNounsInfoFromDB error: Fail while open file." << endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, noun, frequency FROM nouns;";
	
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToFile, &file, &zErrMsg);
  
	file.close();
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetNounsInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

bool DBAccess::GetNounInfoFromDB(int id, std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
	sqlite3_stmt* stmt = 0;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetNounInfoFromDB error: Fail while open file."<< endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, noun, frequency FROM nouns\
			WHERE id = ?";
			
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
  
	file.close();
	
	if( rc != SQLITE_OK ){
		cout << "GetNounInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

std::ostream & DBAccess::GetNounsInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, noun, frequency FROM nouns;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetNounsInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAccess::GetNounInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, noun, frequency FROM nouns WHERE id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetNounInfoFromDB error:" << endl;
	}
	
	return out;
}

std::vector<int> DBAccess::GetNounsIndexes()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT id FROM nouns";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		ids.push_back(sqlite3_column_int(stmt, 0));

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetNounsIndexes error:" << endl;
		return ids;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "GetNounsIndexes error:" << endl;
	else 
		return ids;
}	

string DBAccess::RandomNounFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM nouns";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		table_size = sqlite3_column_int(stmt, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomNounFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomNounFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT noun FROM nouns WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt,1,x);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, 0);
		if(tmp == NULL)
			result = "";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomNounFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomNounFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}

	// REMOVE TASKS:
bool DBAccess::RemoveNounFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM nouns WHERE id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveNounFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM nouns WHERE id = '" + to_string(id) + "';";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveNounFromDB error:" << endl;
		return false;
	}
	else
		return true;
}

	// SET VALUES:
void DBAccess::SetNoun(int id, std::string noun)
{
	SetNounRecord(id, 1, "noun", noun);
}

void DBAccess::SetNounFrequency(int id, int frequency)
{
	SetNounRecord(id, 1, "frequency", to_string(frequency));
}

void DBAccess::SetNounRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM nouns WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char *tmp = sqlite3_column_text( stmt, columnx );
		string tmp2 = std::string(reinterpret_cast<const char*>(tmp));
    }
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetNounRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetNounRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE nouns SET " + column + " = ?  WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_text(stmt,1,new_value.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,id);
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddNounToDB (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetNounRecord error:" << endl;
}


// VERBS
	// ADD VERB:
int  DBAccess::AddVerbToDB(string verb, int frequency)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM verbs WHERE verb = ? AND frequency = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,verb.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddVerbToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO verbs (verb, frequency) VALUES (?,?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,verb.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddVerbToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT id FROM verbs WHERE verb = \'" + verb + "\' AND frequency = " + to_string(frequency) + " ;";
	
	int newId;
	
	rc = sqlite3_exec(db, sql.c_str(), GetIdFromDB,&newId, &zErrMsg);
	
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL GetIdFromDB error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	} 
	else
			return newId;
		
   return -1;
}	

bool	DBAccess::BuildVerbsTable()
{	
	int rc; // This line
	string sql; // This line
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists \"verbs\" ( \"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, \"verb\" TEXT NOT NULL,\"frequency\"	INTEGER NOT NULL DEFAULT 0);";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildVerbsTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAccess::ClearVerbsTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM verbs";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearVerbsTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearVerbsTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAccess::GetVerb(int id)
{
	return GetVerbsRecord(id,1);
}

int DBAccess::GetVerbFrequency(int id)
{
	return stoi(GetVerbsRecord(id,2));
}

string DBAccess::GetVerbsRecord(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM verbs WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, column);
		if(tmp == NULL)
			result = "0";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "GetVerbsRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

bool DBAccess::GetVerbsInfoFromDB(std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetVerbsInfoFromDB error: Fail while open file." << endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, verb, frequency FROM verbs;";
	
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToFile, &file, &zErrMsg);
  
	file.close();
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetVerbsInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

bool DBAccess::GetVerbInfoFromDB(int id, std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
	sqlite3_stmt* stmt = 0;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetVerbInfoFromDB error: Fail while open file."<< endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, verb, frequency FROM verbs\
			WHERE id = ?";
			
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
  
	file.close();
	
	if( rc != SQLITE_OK ){
		cout << "GetVerbInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

std::ostream & DBAccess::GetVerbsInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, verb, frequency FROM verbs;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetVerbsInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAccess::GetVerbInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, verb, frequency FROM verbs WHERE id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetVerbInfoFromDB error:" << endl;
	}
	
	return out;
}

std::vector<int> DBAccess::GetVerbsIndexes()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT id FROM verbs";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		ids.push_back(sqlite3_column_int(stmt, 0));

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetVerbsIndexes error:" << endl;
		return ids;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "GetVerbsIndexes error:" << endl;
	else 
		return ids;
}	


string DBAccess::RandomVerbFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM verbs";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		table_size = sqlite3_column_int(stmt, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomVerbFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomVerbFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT verb FROM verbs WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt,1,x);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, 0);
		if(tmp == NULL)
			result = "";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomVerbFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomVerbFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}


	// REMOVE TASKS:
bool DBAccess::RemoveVerbFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM verbs WHERE id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveVerbFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM verbs WHERE id = '" + to_string(id) + "';";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveVerbFromDB error:" << endl;
		return false;
	}
	else
		return true;
}

	// SET VALUES:
void DBAccess::SetVerb(int id, std::string verb)
{
	SetVerbRecord(id, 1, "verb", verb);
}

void DBAccess::SetVerbFrequency(int id, int frequency)
{
	SetVerbRecord(id, 1, "frequency", to_string(frequency));
}

void DBAccess::SetVerbRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM verbs WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char *tmp = sqlite3_column_text( stmt, columnx );
		string tmp2 = std::string(reinterpret_cast<const char*>(tmp));
    }
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetVerbRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetVerbRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE verbs SET " + column + " = ?  WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_text(stmt,1,new_value.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,id);
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddVerbToDB (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetVerbRecord error:" << endl;
}


// ADJECTIVES
	// ADD ADJECTIVES:
int  DBAccess::AddAdjectiveToDB(string adjective, int frequency)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM adjectives WHERE adjective = ? AND frequency = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,adjective.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO adjectives (adjective, frequency) VALUES (?,?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,adjective.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddAdjectiveToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT id FROM adjectives WHERE adjective = \'" + adjective + "\' AND frequency = " + to_string(frequency) + " ;";
	
	int newId;
	
	rc = sqlite3_exec(db, sql.c_str(), GetIdFromDB,&newId, &zErrMsg);
	
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL GetIdFromDB error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	} 
	else
			return newId;
		
   return -1;
}	

bool	DBAccess::BuildAdjectivesTable()
{	
	int rc; // This line
	string sql; // This line
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists \"adjectives\" ( \"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, \"adjective\" TEXT NOT NULL,\"frequency\"	INTEGER NOT NULL DEFAULT 0);";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildVerbsTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAccess::ClearAdjectivesTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM adjectives";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearAdjectivesTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearAdjectivesTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAccess::GetAdjective(int id)
{
	return GetAdjectivesRecord(id,1);
}

int DBAccess::GetAdjectiveFrequency(int id)
{
	return stoi(GetAdjectivesRecord(id,2));
}

string DBAccess::GetAdjectivesRecord(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM adjectives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, column);
		if(tmp == NULL)
			result = "0";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "GetAdjectivesRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

bool DBAccess::GetAdjectivesInfoFromDB(std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetAdjectivesInfoFromDB error: Fail while open file." << endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, adjective, frequency FROM adjectives;";
	
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToFile, &file, &zErrMsg);
  
	file.close();
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAdjectivesInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

bool DBAccess::GetAdjectiveInfoFromDB(int id, std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
	sqlite3_stmt* stmt = 0;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetAdjectiveInfoFromDB error: Fail while open file."<< endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, adjective, frequency FROM adjectives\
			WHERE id = ?";
			
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
  
	file.close();
	
	if( rc != SQLITE_OK ){
		cout << "GetAdjectiveInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

std::ostream & DBAccess::GetAdjectivesInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, adjective, frequency FROM adjectives;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAdjectivesInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAccess::GetAdjectiveInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, adjective, frequency FROM adjectives WHERE id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAdjectiveInfoFromDB error:" << endl;
	}
	
	return out;
}

std::vector<int> DBAccess::GetAdjectivesIndexes()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT id FROM adjectives";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		ids.push_back(sqlite3_column_int(stmt, 0));

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAdjectivesIndexes error:" << endl;
		return ids;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "GetAdjectivesIndexes error:" << endl;
	else 
		return ids;
}	


string DBAccess::RandomAdjectiveFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM adjectives";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		table_size = sqlite3_column_int(stmt, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomAdjectiveFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomAdjectiveFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT adjective FROM adjectives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt,1,x);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, 0);
		if(tmp == NULL)
			result = "";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomAdjectiveFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomAdjectiveFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}

// REMOVE TASKS:
bool DBAccess::RemoveAdjectiveFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM adjectives WHERE id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveAdjectiveFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM adjectives WHERE id = '" + to_string(id) + "';";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveAdjectiveFromDB error:" << endl;
		return false;
	}
	else
		return true;
}
// SET VALUES:
void DBAccess::SetAdjective(int id, std::string adjective)
{
	SetAdjectiveRecord(id, 1, "adjective", adjective);
}

void DBAccess::SetAdjectiveFrequency(int id, int frequency)
{
	SetAdjectiveRecord(id, 1, "frequency", to_string(frequency));
}

void DBAccess::SetAdjectiveRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM adjectives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char *tmp = sqlite3_column_text( stmt, columnx );
		string tmp2 = std::string(reinterpret_cast<const char*>(tmp));
    }
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetAdjectiveRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetAdjectiveRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE adjectives SET " + column + " = ?  WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_text(stmt,1,new_value.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,id);
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddAdjectiveToDB (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetAdjectiveRecord error:" << endl;
}


// CONJECTIVE:
int  DBAccess::AddConjunctiveToDB(string conjunctive, int frequency)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM conjunctives WHERE conjunctive = ? AND frequency = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,conjunctive.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO conjunctives (conjunctive, frequency) VALUES (?,?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,conjunctive.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,frequency);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddConjunctiveToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT id FROM conjunctives WHERE conjunctive = \'" + conjunctive + "\' AND frequency = " + to_string(frequency) + " ;";
	
	int newId;
	
	rc = sqlite3_exec(db, sql.c_str(), GetIdFromDB,&newId, &zErrMsg);
	
	if( rc != SQLITE_OK )
	{
		fprintf(stderr, "SQL GetIdFromDB error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
		return -1;
	} 
	else
			return newId;
		
   return -1;
}	

bool	DBAccess::BuildConjunctivesTable()
{	
	int rc; // This line
	string sql; // This line
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists \"conjunctives\" ( \"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE, \"conjunctive\" TEXT NOT NULL,\"frequency\"	INTEGER NOT NULL DEFAULT 0);";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildVerbsTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAccess::ClearConjunctivesTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM conjunctives";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearConjunctivesTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearConjunctivesTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAccess::GetConjunctive(int id)
{
	return GetConjunctivesRecord(id,1);
}

int DBAccess::GetConjunctiveFrequency(int id)
{
	return stoi(GetConjunctivesRecord(id,2));
}

string DBAccess::GetConjunctivesRecord(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM conjunctives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, column);
		if(tmp == NULL)
			result = "0";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "GetConjunctivesRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

bool DBAccess::GetConjunctivesInfoFromDB(std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetConjunctivesInfoFromDB error: Fail while open file." << endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, conjunctive, frequency FROM conjunctives;";
	
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToFile, &file, &zErrMsg);
  
	file.close();
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetConjunctivesInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

bool DBAccess::GetConjunctiveInfoFromDB(int id, std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
	sqlite3_stmt* stmt = 0;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetConjunctiveInfoFromDB error: Fail while open file."<< endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, conjunctive, frequency FROM conjunctives\
			WHERE id = ?";
			
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
  
	file.close();
	
	if( rc != SQLITE_OK ){
		cout << "GetConjunctiveInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

std::ostream & DBAccess::GetConjunctivesInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, conjunctive, frequency FROM conjunctives;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetConjunctivesInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAccess::GetConjunctiveInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, conjunctive, frequency FROM conjunctives WHERE id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetConjunctiveInfoFromDB error:" << endl;
	}
	
	return out;
}

std::vector<int> DBAccess::GetConjunctivesIndexes()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT id FROM conjunctives";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		ids.push_back(sqlite3_column_int(stmt, 0));

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetConjunctivesIndexes error:" << endl;
		return ids;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "GetConjunctivesIndexes error:" << endl;
	else 
		return ids;
}	


string DBAccess::RandomConjunctiveFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM conjunctives";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
		table_size = sqlite3_column_int(stmt, 0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomConjunctiveFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomConjunctiveFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT conjunctive FROM conjunctives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 

	sqlite3_bind_int(stmt,1,x);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char * tmp = sqlite3_column_text(stmt, 0);
		if(tmp == NULL)
			result = "";
		else
			result = std::string(reinterpret_cast<const char*>(tmp));
    }

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RandomConjunctiveFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomConjunctiveFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}

	// REMOVE TASKS:
bool DBAccess::RemoveConjunctiveFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM conjunctives WHERE id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveConjunctiveFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM conjunctives WHERE id = '" + to_string(id) + "';";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_int(stmt,1,id);
	
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveConjunctiveFromDB error:" << endl;
		return false;
	}
	else
		return true;
}

	// SET VALUES:
void DBAccess::SetConjunctive(int id, std::string conjunctive)
{
	SetConjunctiveRecord(id, 1, "conjunctive", conjunctive);
}

void DBAccess::SetConjunctiveFrequency(int id, int frequency)
{
	SetConjunctiveRecord(id, 1, "frequency", to_string(frequency));
}

void DBAccess::SetConjunctiveRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM conjunctives WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_int(stmt,1,id);
	
	while ( sqlite3_step( stmt ) == SQLITE_ROW )
	{
		const unsigned char *tmp = sqlite3_column_text( stmt, columnx );
		string tmp2 = std::string(reinterpret_cast<const char*>(tmp));
    }
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetConjunctiveRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetConjunctiveRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE conjunctives SET " + column + " = ?  WHERE id = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), sql.length(), &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 );
	
	sqlite3_bind_text(stmt,1,new_value.c_str(),-1,0);
	sqlite3_bind_int(stmt,2,id);
  
	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddConjunctiveToDB (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetConjunctiveRecord error:" << endl;
}


// USED IN OTHER FUNCTION:
int DBAccess::IsAlreadyInTable(void* isInTable, int argc, char **argv, char **azColName)
{	
	bool* tmp = (bool*) isInTable;
	
	if(argc >= 0)
		*tmp = true;
	else 
		*tmp = false;
	return 0;
}

int DBAccess::Callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int DBAccess::GetIdFromDB(void *id, int argc, char **argv, char **azColName)
{
	int* tmp;
	tmp = (int*) id;
	*tmp = atoi(argv	[0]);
	return 0;
}

int DBAccess::PrintFromDBToFile(void *file, int argc, char **argv, char **azColName) 
{
	int i;
	fstream* tmp;
	tmp = (fstream*) file;
	
	for(i = 0; i<argc; i++)
		*tmp << azColName[i] << " = "  << (argv[i] ? argv[i] : "NULL") << "\n";
	
	*tmp << endl;
	
	return 0;
}

int DBAccess::PrintFromDBToStream(void *out, int argc, char **argv, char **azColName) 
{
	int i;
	ostream* tmp;
	tmp = (ostream*) out;
	
	for(i = 0; i<argc; i++)
		*tmp << azColName[i] << " = "  << (argv[i] ? argv[i] : "NULL") << "\n";
	
	*tmp << endl;
	
	return 0;
}


	// GET VALUES FROM RECORDS:
int DBAccess::GetRecordFromDB(void *record, int argc, char **argv, char **azColName) 
{
	string* tmp;
	tmp = (string*) record;
	if(argv[0] == NULL)
		*tmp = "NULL";
	else
		*tmp = argv[0];
	return 0;
}	

DBAccess::~DBAccess()
{
	sqlite3_close(db);
}

int DBAccess::RandID(int table_size)
{
	int x;
	float y;
	std::random_device rd;
    std::default_random_engine e2(rd()) ;
    std::uniform_real_distribution<> dist(0, 1);
	
	y = dist(e2);
	//cout << "ts = " << table_size << endl;
	//cout << "y = " << y << endl;
	//cout << "ts^(1/6) = " << pow((double)y,((float)1.0)/(float(7.0))) << endl;
	
	x = (int) table_size*(pow((double)y,((float)1.0)/(float(7.0))) );
	//cout << "x = " << x << endl;
	
	return x;
}