#ifndef TESTS_HPP
#define TESTS_HPP
	
#include "DBAccess.h"
#include "Sentence1.h"
#include "Sentence2.h"
#include "Sentence3.h"
#include "SentenceGenerator.h"
#include "CensorshipProgram.h"
#include "Messenger.h"
#include <semaphore.h>
#include <signal.h>
#include <pthread.h> 
#include <unistd.h> 
#include <iostream>
#include <sys/stat.h>

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>

class Tests
{
private:
	sem_t* mutex;
    public:
        bool testAll();
		bool DBATests();
		bool SentencesTests();
		bool CensorshipTests();
		bool MessengerTests();
		void initializeMutex();
		bool archiveTests();
		bool synchroTests();
		bool DBAATests();
};

struct exceptionData
{
    std::string thrownClass;
    std::string thrownFunction;
    std::string thrownStatement;
};

std::ostream & operator<< (std::ostream &out, const exceptionData &exc);

exceptionData PreperExceptionData(std::string className, std::string functionName, std::string statement);

#endif // TESTS_HPP
