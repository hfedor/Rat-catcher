#include "DBAchivesAccess.h"

using namespace std;

// CONSTRUCTOR:
DBAchivesAccess::DBAchivesAccess(string db_file_name)
{
	int rc = sqlite3_open(db_file_name.c_str(), &db);
	
	if( rc )
	{
		string message = "Can't open database: ";
		message += sqlite3_errmsg(db);
		cout << "DBAchivesAccess: " + message << endl;
	}
	else
		BuildAchivesTable();
}

// ADD MESSAGE:
int  DBAchivesAccess::AddMessageToDB(string message, string sended, string censored, string loaded)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM achives WHERE message = ? AND sended = ? AND censored = ? AND loaded = ?";
	
	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,message.c_str(),-1,0);
	sqlite3_bind_text(stmt,2,sended.c_str(),-1,0);
	sqlite3_bind_text(stmt,3,censored.c_str(),-1,0);
	sqlite3_bind_text(stmt,4,loaded.c_str(),-1,0);

	if( sqlite3_step( stmt ) == SQLITE_ROW ) 
		isInTable = true;

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "AddMessageToDB (IsAlreadyInTable) error:" << endl;
		return -1;
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
		cout << "AddMessageToDB (IsAlreadyInTable) error:" << endl;
	else if(isInTable)
	{
		cout << "AddMessageToDB (IsAlreadyInTable) error: Recored already in table." << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "INSERT INTO achives (message, sended, cenosred, loaded) VALUES (?,?, ?, ?)";

	sqlite3_prepare_v2( db, sql.c_str(), -1, &stmt, 0 );

	sqlite3_exec( db, "BEGIN TRANSACTION", 0, 0, 0 ); 
	
	sqlite3_bind_text(stmt,1,message.c_str(),-1,0);
	sqlite3_bind_text(stmt,2,sended.c_str(),-1,0);
	sqlite3_bind_text(stmt,3,censored.c_str(),-1,0);
	sqlite3_bind_text(stmt,4,loaded.c_str(),-1,0);

	sqlite3_step( stmt );
	sqlite3_clear_bindings( stmt );
	sqlite3_reset( stmt );
	
	rc = sqlite3_exec( db, "END TRANSACTION", 0, 0, &zErrMsg );   //  End the transaction.
	
	if( rc != SQLITE_OK )
	{;
		sqlite3_free(zErrMsg);
		cout << "AddMessageToDB error:" << endl;
	}
	
	rc = sqlite3_finalize( stmt );
   
	if( rc != SQLITE_OK ){
		cout << "AddMessageToDB error:" << endl;
		return -1;
	}
	
	// Create SQL statement
	sql = "SELECT id FROM achives WHERE message = \'" + message + "\' AND sended = \'" + sended + "\' AND sended = \'" + censored + "\' AND sended = \'" + loaded + "\' ;";
	
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

bool	DBAchivesAccess::BuildAchivesTable()
{	
	int rc; // This line
	string sql; // This line
	sqlite3_stmt* stmt = 0;
	
	/* Create SQL statement */
	sql = "CREATE TABLE if not exists  \"achives\" (\"id\"	INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT UNIQUE,\"message\"	TEXT NOT NULL,\"sended\"	TEXT NOT NULL,\"censored\"	TEXT NOT NULL,\"loaded\"	TEXT NOT NULL);";
	
	rc = sqlite3_exec(db, sql.c_str(), 0, 0, 0);

	if( rc != SQLITE_OK ){
		cout << "BuildAchivesTable error: CREATE" << endl;
		return false;
	}
	else
		return true;
}

bool DBAchivesAccess::ClearAchivesTable()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;

	sql = "DELETE FROM achives";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "ClearAchivesTable error:" << endl;
		return false;	
	}
	
	sql = "VACUUM";

	rc = sqlite3_exec(db, sql.c_str(), 0, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		cout << "ClearAchivesTable error:" << endl;
		return false;
	}
	
	return true;
}

std::string DBAchivesAccess::GetMessage(int id2)
{
	return GetRecord(id2,1);
}

std::string DBAchivesAccess::GetSended(int id)
{
	return GetRecord(id,2);
}

std::string DBAchivesAccess::GetCensored(int id)
{
	return GetRecord(id,3);
}

std::string DBAchivesAccess::GetLoaded(int id)
{
	return GetRecord(id,4);
}

string DBAchivesAccess::GetRecord	(int id, int column)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	string result;
	sqlite3_stmt* stmt = 0;

	sql = "SELECT * FROM achives WHERE id = ?";
	
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
		cout << "SetRecord error:" << endl;
		return "0";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK ){
		cout << "SetRecord error:" << endl;
		return "0";
	}
	else 
		return result;
}

bool DBAchivesAccess::GetAchivesInfoFromDB(std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetAchivesInfoFromDB error: Fail while open file." << endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, message, sended, censored, loaded FROM achives;";
	
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToFile, &file, &zErrMsg);
  
	file.close();
	
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAchivesInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

bool DBAchivesAccess::GetAchiveInfoFromDB(int id, std::string file_name)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	fstream file;
	sqlite3_stmt* stmt = 0;
   
	file.open( file_name, std::ios::out);
	if( !file.good() )
	{
		cout << "GetAchiveInfoFromDB error: Fail while open file."<< endl;
		return false;
	}
	
	// Create SQL statement
	sql = "SELECT id, message, sended, censored, loaded FROM achives\
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
		cout << "GetAchiveInfoFromDB error:" << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
  
	file.close();
	
	if( rc != SQLITE_OK ){
		cout << "GetAchiveInfoFromDB error:" << endl;
		return false;
	}
	else 
		return true;
}

std::ostream & DBAchivesAccess::GetAchivesInfoFromDB(std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, message, sended, censored, loaded FROM achives;";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAchivesInfoFromDB error:" << endl;
    }
	return out;
}

std::ostream & DBAchivesAccess::GetAchiveInfoFromDB(int id, std::ostream &out)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
   
	sql = "SELECT id, message, sended, censored, loaded FROM achives WHERE id = " + to_string(id) + ";";
		
	rc = sqlite3_exec(db, sql.c_str(), PrintFromDBToStream, &out, &zErrMsg);
	  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "GetAchiveInfoFromDB error:" << endl;
	}
	
	return out;
}

std::vector<int> DBAchivesAccess::GetAchivesIndexes()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	sqlite3_stmt* stmt = 0;
	vector<int> ids;

	sql = "SELECT id FROM achives";
	
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
		cout << "GetAchivesIndexes error:" << endl;
		return ids;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "GetAchivesIndexes error:" << endl;
	else 
		return ids;
}	

string DBAchivesAccess::RandomMessageFromDB()
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	int table_size;
	sqlite3_stmt* stmt = 0;
	string result;
	
	// Create SQL statement
	sql = "SELECT COUNT(*) FROM achives";
	
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
		cout << "RandomMessageFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomMessageFromDB : Get rable size" << endl;
		return "";
	}
	
	int x = RandID(table_size);
	
	// Create SQL statement
	sql = "SELECT message FROM achives WHERE id = ?";
	
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
		cout << "RandomMessageFromDB error:" << endl;
		return "";
	}
	
	rc = sqlite3_finalize( stmt );
 
	if( rc != SQLITE_OK )
	{
		cout << "RandomMessageFromDB error: Get record by index." << endl;
		return "";
	}
	
	return result;
}

	// REMOVE TASKS:
bool DBAchivesAccess::RemoveMessageFromDB(int id)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM achives WHERE id = ?";
	
	rc = sqlite3_exec(db, sql.c_str(), IsAlreadyInTable,&isInTable, &zErrMsg);
  
	if( rc != SQLITE_OK )
	{
		sqlite3_free(zErrMsg);
		cout << "RemoveMessageFromDB (IsAlreadyInTable) error:" << endl;
		return false;
    }
	
	// Create SQL statement
	sql = "DELETE FROM achives WHERE id = '" + to_string(id) + "';";
	
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
		cout << "AddMessageToDB (IsAlreadyInTable) error:"  << endl;
		return false;
	}
	
	rc = sqlite3_finalize( stmt );
	  
	if( rc != SQLITE_OK ){
		cout << "RemoveMessageFromDB error:" << endl;
		return false;
	}
	else
		return true;
}

	// SET VALUES:
void DBAchivesAccess::SetMessage(int id, std::string message)
{
	SetRecord(id, 1, "message", message);
}

void DBAchivesAccess::SetSended(int id, string sended)
{
	SetRecord(id, 1, "sended", sended);
}

void DBAchivesAccess::SetRecord(int id, int columnx, std::string column, std::string new_value)
{
	char *zErrMsg = 0;
	int rc;
	string sql;
	bool isInTable = false;
	sqlite3_stmt* stmt = 0;

	// Create SQL statement
	sql = "SELECT * FROM achives WHERE id = ?";
	
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
		cout << "AddMessageToDB (IsAlreadyInTable) error:" << endl;
		
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK ){
		cout << "SetRecord (IsAlreadyInTable) error:" << endl;
		return;
	}
	else if(isInTable){
		cout << "SetRecord (IsAlreadyInTable) error: Recored already has given value" << endl;
		return;
	}
	sql = "UPDATE achives SET " + column + " = ?  WHERE id = ?";
	
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
		cout << "AddMessageToDB (IsAlreadyInTable) error:" << endl;
		return;
	}
	
	rc = sqlite3_finalize( stmt );
	
	if( rc != SQLITE_OK )
		cout << "SetRecord error:" << endl;
}


// USED IN OTHER FUNCTION:
int DBAchivesAccess::IsAlreadyInTable(void* isInTable, int argc, char **argv, char **azColName)
{	
	bool* tmp = (bool*) isInTable;
	
	if(argc >= 0)
		*tmp = true;
	else 
		*tmp = false;
	return 0;
}

int DBAchivesAccess::Callback(void *NotUsed, int argc, char **argv, char **azColName)
{
	int i;
	for(i = 0; i<argc; i++) {
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int DBAchivesAccess::GetIdFromDB(void *id, int argc, char **argv, char **azColName)
{
	int* tmp;
	tmp = (int*) id;
	*tmp = atoi(argv	[0]);
	return 0;
}

int DBAchivesAccess::PrintFromDBToFile(void *file, int argc, char **argv, char **azColName) 
{
	int i;
	fstream* tmp;
	tmp = (fstream*) file;
	
	for(i = 0; i<argc; i++)
		*tmp << azColName[i] << " = "  << (argv[i] ? argv[i] : "NULL") << "\n";
	
	*tmp << endl;
	
	return 0;
}

int DBAchivesAccess::PrintFromDBToStream(void *out, int argc, char **argv, char **azColName) 
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
int DBAchivesAccess::GetRecordFromDB(void *record, int argc, char **argv, char **azColName) 
{
	string* tmp;
	tmp = (string*) record;
	if(argv[0] == NULL)
		*tmp = "NULL";
	else
		*tmp = argv[0];
	return 0;
}	

DBAchivesAccess::~DBAchivesAccess()
{
	sqlite3_close(db);
}

int DBAchivesAccess::RandID(int table_size)
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