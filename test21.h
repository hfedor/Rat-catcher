#pragma once
#ifndef TEST21_H
#define TEST21_H
	
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

using namespace std;

class test21
{
private:
	sem_t* mutex;
    public:
    		void test();
		void initializeMutex();
		void consumer();
		void producer();
		
		
};


#endif // TEST21_H
