#ifndef DBACCESS_H
#define DBACCESS_H

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
#include <chrono>
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
		int RandID(int table_size);

	public:
		// FUNCTIONS TO CONNECT WITH DATEBASE:
		
		// TASK INFO:
		DBAccess(std::string db_file_name);
		int AddNounToDB(std::string noun, int frequency); // add noun with gien datas to database
		bool BuildNounsTable(); // build  noun table
		bool ClearNounsTable(); // remove all records from tasks table
		std::string GetNoun(int noun_id); // get path to file of noun with given ID
		int GetNounFrequency(int noun_id); // get path to file of noun with given ID
		std::string GetNounsRecord(int noun_id, int column);  // get record from given column from node with given ID
		bool GetNounsInfoFromDB(std::string file_name); // get Inforamtions about all noun and put it to file with given name
		bool GetNounInfoFromDB(int noun_id, std::string file_name); // get Inforamtions about noun with given ID and put it to file with given name
		std::vector<int> GetNounsIndexes(); // get vector of tasks indexes
		std::ostream & GetNounsInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetNounInfoFromDB(int noun_id, std::ostream &out); // get Inforamtions about noun with given ID and put it to given stream
		std::string RandomNounFromDB();
		bool RemoveNounFromDB(int noun_id); // remove noun with given ID from database and return if removing sucesed
		void SetNoun(int noun_id, std::string noun); // set new path to file of noun copy with gicen ID
		void SetNounFrequency(int noun_id, int frequency); // set new path to file of noun copy with gicen ID
		void SetNounRecord(int noun_id, int columnx, std::string column, std::string new_value); // set new given value of given column in noun with gicen ID
		
		int AddVerbToDB(std::string verb, int frequency); // add verb with gien datas to database
		bool BuildVerbsTable(); // build  verb table
		bool ClearVerbsTable(); // remove all records from tasks table
		std::string GetVerb(int verb_id); // get path to file of verb with given ID
		int GetVerbFrequency(int verb_id); // get path to file of verb with given ID
		std::string GetVerbsRecord(int verb_id, int column);  // get record from given column from node with given ID
		bool GetVerbsInfoFromDB(std::string file_name); // get Inforamtions about all verb and put it to file with given name
		bool GetVerbInfoFromDB(int verb_id, std::string file_name); // get Inforamtions about verb with given ID and put it to file with given name
		std::vector<int> GetVerbsIndexes(); // get vector of tasks indexes
		std::ostream & GetVerbsInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetVerbInfoFromDB(int verb_id, std::ostream &out); // get Inforamtions about verb with given ID and put it to given stream
		std::string RandomVerbFromDB();
		bool RemoveVerbFromDB(int verb_id); // remove verb with given ID from database and return if removing sucesed
		void SetVerb(int verb_id, std::string verb); // set new path to file of verb copy with gicen ID
		void SetVerbFrequency(int verb_id, int frequency); // set new path to file of verb copy with gicen ID
		void SetVerbRecord(int verb_id, int columnx, std::string column, std::string new_value); // set new given value of given column in verb with gicen ID
		
		int AddAdjectiveToDB(std::string adjective, int frequency); // add adjective with gien datas to database
		bool BuildAdjectivesTable(); // build  adjective table
		bool ClearAdjectivesTable(); // remove all records from tasks table
		std::string GetAdjective(int adjective_id); // get path to file of adjective with given ID
		int GetAdjectiveFrequency(int adjective_id); // get path to file of adjective with given ID
		std::string GetAdjectivesRecord(int adjective_id, int column);  // get record from given column from node with given ID
		bool GetAdjectivesInfoFromDB(std::string file_name); // get Inforamtions about all adjective and put it to file with given name
		bool GetAdjectiveInfoFromDB(int adjective_id, std::string file_name); // get Inforamtions about adjective with given ID and put it to file with given name
		std::vector<int> GetAdjectivesIndexes(); // get vector of tasks indexes
		std::ostream & GetAdjectivesInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetAdjectiveInfoFromDB(int adjective_id, std::ostream &out); // get Inforamtions about adjective with given ID and put it to given stream
		std::string RandomAdjectiveFromDB();
		bool RemoveAdjectiveFromDB(int adjective_id); // remove adjective with given ID from database and return if removing sucesed
		void SetAdjective(int adjective_id, std::string adjective); // set new path to file of adjective copy with gicen ID
		void SetAdjectiveFrequency(int adjective_id, int frequency); // set new path to file of adjective copy with gicen ID
		void SetAdjectiveRecord(int adjective_id, int columnx, std::string column, std::string new_value); // set new given value of given column in adjective with gicen ID
		
		int AddConjunctiveToDB(std::string conjunctive, int frequency); // add conjunctive with gien datas to database
		bool BuildConjunctivesTable(); // build  conjunctive table
		bool ClearConjunctivesTable(); // remove all records from tasks table
		std::string GetConjunctive(int conjunctive_id); // get path to file of conjunctive with given ID
		int GetConjunctiveFrequency(int conjunctive_id); // get path to file of conjunctive with given ID
		std::string GetConjunctivesRecord(int conjunctive_id, int column);  // get record from given column from node with given ID
		bool GetConjunctivesInfoFromDB(std::string file_name); // get Inforamtions about all conjunctive and put it to file with given name
		bool GetConjunctiveInfoFromDB(int conjunctive_id, std::string file_name); // get Inforamtions about conjunctive with given ID and put it to file with given name
		std::vector<int> GetConjunctivesIndexes(); // get vector of tasks indexes
		std::ostream & GetConjunctivesInfoFromDB(std::ostream &out); // get Inforamtions about all tasks and put it to given stream
		std::ostream & GetConjunctiveInfoFromDB(int conjunctive_id, std::ostream &out); // get Inforamtions about conjunctive with given ID and put it to given stream
		std::string RandomConjunctiveFromDB();
		bool RemoveConjunctiveFromDB(int conjunctive_id); // remove conjunctive with given ID from database and return if removing sucesed
		void SetConjunctive(int conjunctive_id, std::string conjunctive); // set new path to file of conjunctive copy with gicen ID
		void SetConjunctiveFrequency(int conjunctive_id, int frequency); // set new path to file of conjunctive copy with gicen ID
		void SetConjunctiveRecord(int conjunctive_id, int columnx, std::string column, std::string new_value); // set new given value of given column in conjunctive with gicen ID
		
		
		~DBAccess();
};

std::string return_current_time_and_date();
	
#endif // PMDATEBASEACCESS_HPP