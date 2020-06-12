#include "test21.h"
using namespace std;

void test21::test()
{
	initializeMutex();

	pid_t process_id_1;
	
	process_id_1 = fork(); // fork
	
	if(process_id_1 == 0)
	{
		test21::consumer(); // consumer

	}
		
	else if(process_id_1 > 0)
	{
		test21::producer(); // producer
	}
	
	else if(process_id_1 < 0)
		std::cout << "error" << std::endl;


}

void test21::initializeMutex()
{
	sem_init(test21::mutex, 1, 1); // First argument is a pointer to our mutex, second one is wether its between processes or threads, third one is init value
}

void test21::consumer()
{
	std::cout << std::endl << "I am consumer" << std::endl;
	CensorshipProgram cp;
	
	cp.generateForbidens(10);
	
	cp.printForbidens();
	
	sem_wait(test21::mutex); // We eneter the section
	
	cp.loadMessage("messages_test.txt");
	
	sem_post(test21::mutex); // We leave the section
	
	cp.printMessage();
	
	cp.censureMessage();
	cout << "censored:" << endl;
	cp.printCensored();
	
	cp.loadMessage();
	
	cp.printMessage();
	
	cp.censureMessage();
	cout << "censored:" << endl;
	cp.printCensored();
}
void test21::producer()
{
	std::cout << std::endl << "I am producer" << std::endl;
	
	Messenger messenger("Producent");
	
	messenger.generateMessage();
	
	cout << messenger << endl;
	
	sem_wait(test21::mutex); // We eneter the section
	
	messenger.sendMessage("messages_test.txt");
	
	sem_post(test21::mutex); // We leave the section
	
}


