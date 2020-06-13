#ifndef DBMESSAGEACCESS_H
#define DBMESSAGEACCESS_H

#include <iostream>
#include <sqlite3.h> 
#include <string>
#include <cstdint>
#include <fstream>
#include <vector>
#include <math.h>
#include <stdio.h>
#include <bits/stdc++.h>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_real_distribution.hpp>
#include <random>
/*
	Class to get access to database
*/

class DBMessageAccess
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
		int RandID(int table_size);
	public:
		// FUNCTIONS TO CONNECT WITH DATEBASE:
		
		// TASK INFO:
		DBMessageAccess(std::string db_file_name);
		int AddMessageToDB(std::string noun, std::string sented); // add noun with gien datas to database
		bool BuildMessagesTable(); // build  noun table
		bool ClearMessagesTable(); // remove all records from tasks table
		int FindFirst();
		std::string GetMessage(int noun_id); // get path to file of noun with given ID
		std::string GetMessageSended(int noun_id); // get path to file of noun with given ID
		std::string GetMessagesRecord(int noun_id, int column);  // get record from given column from node with given ID
		bool GetMessagesInfoFromDB(std::string file_name); // get Inforamtions about all noun and put it to file with given name
		bool GetMessageInfoFromDB(int noun_id, std::string file_name); // get Inforamtions about noun with given ID and put it to file with given name
		std::vector<int> GetMessagesIndexes(); // get vector of tasks indexes
		std::ostream & GetMessagesInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetMessageInfoFromDB(int noun_id, std::ostream &out); // get Inforamtions about noun with given ID and put it to given stream
		std::string RandomMessageFromDB();
		bool RemoveMessageFromDB(int noun_id); // remove noun with given ID from database and return if removing sucesed
		void SetMessage(int noun_id, std::string noun); // set new path to file of noun copy with gicen ID
		void SetMessageSended(int noun_id, std::string sended); // set new path to file of noun copy with gicen ID
		void SetMessageRecord(int noun_id, int columnx, std::string column, std::string new_value); // set new given value of given column in noun with gicen ID
		
		~DBMessageAccess();
};
	
#endif // PMDATEBASEACCESS_HPP