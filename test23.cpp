#include "test23.h"
using namespace std;

void test23::test()
{
	initializeMutex();

	pid_t process_id_1;

	for(int i = 0; i < 5; i++)
	{
		process_id_1 = fork(); // fork
		
		if(process_id_1 == 0)
		{
			test23::consumer(); // consumer

		}
			
		else if(process_id_1 > 0)
		{
			test23::producer(); // producer
		}
		
		else if(process_id_1 < 0)
			std::cout << "error" << std::endl;

	}
}

void test23::initializeMutex()
{
	sem_init(test23::mutex, 1, 1); // First argument is a pointer to our mutex, second one is wether its between processes or threads, third one is init value
}

void test23::consumer()
{
	
	struct sched_param parametr = { .sched_priority = sched_get_priority_max(SCHED_FIFO) }; // We will use it to change our policy to FIFO 
	//int result = sched_setscheduler(0, SCHED_FIFO, &parametr); // FIFO Policy
	//int result = sched_setscheduler(0, SCHED_BATCH, 0); // BATCH Policy
	int result = sched_setscheduler(0, SCHED_OTHER, 0); // BATCH Policy
	
	
	if(result < 0)
		std::cout<<"error while changing priority" << std::endl;
	
	CensorshipProgram cp;
	
	cp.generateForbidens(10);
	
	//cp.printForbidens();
	
	std::cout << std::endl << "I am consumer" << std::endl;
	
	sem_wait(test23::mutex); // We enter the section
		
	cp.loadMessage("messages_test.txt");
		
	sem_post(test23::mutex); // We leave the section
		
	cp.printMessage();
		
	cp.censureMessage();
		//cout << "censored:" << endl;
		//cp.printCensored();
}
void test23::producer()
{
	Messenger messenger("Producent");
	
	struct sched_param parametr = { .sched_priority = sched_get_priority_max(SCHED_FIFO) }; // We will use it to change our policy to FIFO 
	//int result = sched_setscheduler(0, SCHED_FIFO, &parametr); // FIFO Policy
	//int result = sched_setscheduler(0, SCHED_BATCH, 0); // BATCH Policy
	int result = sched_setscheduler(0, SCHED_OTHER, 0); // BATCH Policy
	
	
	if(result < 0)
		std::cout<<"error while changing priority" << std::endl;
	
	std::cout << std::endl << "I am producer" << std::endl;
	
	messenger.generateMessage();
		
	//cout << messenger << endl;
	
	sem_wait(test23::mutex); // We eneter the section
	
	messenger.sendMessage("messages_test.txt");
	
	sem_post(test23::mutex); // We leave the section
}


