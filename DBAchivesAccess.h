#ifndef DBACHIVESACCESS_H
#define DBACHIVESACCESS_H

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
class DBAchivesAccess
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
		DBAchivesAccess(std::string db_file_name);
		int AddMessageToDB(std::string message, std::string sented, std::string cenzored, std::string loaded); // add noun with gien datas to database
		bool BuildAchivesTable(); // build  noun table
		bool ClearAchivesTable(); // remove all records from tasks table
		std::string GetMessage(int message_id); // get path to file of noun with given ID
		std::string GetSended(int message_id); // get path to file of noun with given ID
		std::string GetCensored(int message_id); // get path to file of noun with given ID
		std::string GetLoaded(int message_id); // get path to file of noun with given ID
		std::string GetRecord(int message_id, int column);  // get record from given column from node with given ID
		bool GetAchivesInfoFromDB(std::string file_name); // get Inforamtions about all noun and put it to file with given name
		bool GetAchiveInfoFromDB(int message_id, std::string file_name); // get Inforamtions about noun with given ID and put it to file with given name
		std::vector<int> GetAchivesIndexes(); // get vector of tasks indexes
		std::ostream & GetAchivesInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetAchiveInfoFromDB(int message_id, std::ostream &out); // get Inforamtions about noun with given ID and put it to given stream
		std::string RandomMessageFromDB();
		bool RemoveMessageFromDB(int message_id); // remove noun with given ID from database and return if removing sucesed
		void SetMessage(int message_id, std::string message); // set new path to file of noun copy with gicen ID
		void SetSended(int message_id, std::string sended); // set new path to file of noun copy with gicen ID
		void SetCensored(int message_id, std::string censored); // set new path to file of noun copy with gicen ID
		void SetLoaded(int message_id, std::string loaded); // set new path to file of noun copy with gicen ID
		void SetRecord(int message_id, int columnx, std::string column, std::string new_value); // set new given value of given column in noun with gicen ID
		
		~DBAchivesAccess();
};
	
#endif // PMDATEBASEACCESS_HPP