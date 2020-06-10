#ifndef DBACCESS_H
#define DBACCESS_H

#include <iostream>
#include <sqlite3.h> 
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>

/*
	Class to get access to database
*/

class DBAccess
{
	private:
		sqlite3 *db;
		
		// USED IN OTHER FUNCTION
		static int IsAlreadyInTable(void *NotUsed, int argc, char **argv, char **azColName); // checking if node is already in database
		static int Callback(void *NotUsed, int argc, char **argv, char **azColName);
		static int GetIdFromDB(void *NotUsed, int argc, char **argv, char **azColName);
		static int GetRecordFromDB(void *record, int argc, char **argv, char **azColName);
		static int PrintFromDBToStream(void *out, int argc, char **argv, char **azColName);
		static int PrintFromDBToFile(void *file, int argc, char **argv, char **azColName);
	public:
		// FUNCTIONS TO CONNECT WITH DATEBASE:
		
		// TASK INFO:
		DBAccess(std::string db_file_name);
		int AddNounToDB(std::string noun); // add noun with gien datas to database
		bool BuildNounsTable(); // build  noun table
		bool ClearNounsTable(); // remove all records from tasks table
		std::string GetNoun(int noun_id); // get path to file of noun with given ID
		std::string GetNounsRecord(int noun_id, int column);  // get record from given column from node with given ID
		bool GetNounsInfoFromDB(std::string file_name); // get Inforamtions about all noun and put it to file with given name
		bool GetNounInfoFromDB(int noun_id, std::string file_name); // get Inforamtions about noun with given ID and put it to file with given name
		std::vector<int> GetNounsIndexes(); // get vector of tasks indexes
		std::ostream & GetNounsInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetNounInfoFromDB(int noun_id, std::ostream &out); // get Inforamtions about noun with given ID and put it to given stream
		bool RemoveNounFromDB(int noun_id); // remove noun with given ID from database and return if removing sucesed
		void SetNoun(int noun_id, std::string noun); // set new path to file of noun copy with gicen ID
		void SetNounRecord(int noun_id, int columnx, std::string column, std::string new_value); // set new given value of given column in noun with gicen ID
		~DBAccess();
};
	
#endif // PMDATEBASEACCESS_HPP