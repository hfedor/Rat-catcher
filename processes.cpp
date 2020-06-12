#include <semaphore.h>
#include <stdio.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <cstring>
#include <fcntl.h>
#include <sys/mman.h>

#define SIZE_OF_MEMORY 200


void* sh_mem_creation(size_t s);
void reader();
void writer();
void archive();
// Semafory

sem_t* memory_is_full;
sem_t* memory_is_empty;
sem_t* mutex;
void* our_memory;


int main(int ** argc, char ** argv)
{
	// We need a function that creates shared memory to create our semaphores
	our_memory = sh_mem_creation(SIZE_OF_MEMORY);
	memory_is_full = (sem_t *) sh_mem_creation(sizeof(sem_t));
	memory_is_empty = (sem_t *) sh_mem_creation(sizeof(sem_t));
	mutex = (sem_t *) sh_mem_creation(sizeof(sem_t));
	
	pid_t process_id_1;
	pid_t process_id_2;
	
	process_id_1 = fork(); // double fork
	process_id_2 = fork();
	
	if(process_id_1 == 0)
	{
		if(process_id_2 == 0)
			writer();
		else if(process_id_2 > 0)
			archive();
	}
	
		
	else if(process_id_1 > 0)
	{
		if(process_id_2 == 0)
			reader();
	}
	
	else if(process_id_1 < 0)
		printf("error");
	else if(process_id_2 < 0)
		printf("error");
}


void reader()
{
	char data[SIZE_OF_MEMORY];
	
	sem_wait(memory_is_empty); // If there is something to read we can eneter section
	sem_wait(mutex); // We eneter the section
	
	memcpy(data, our_memory, sizeof(data)); // Mozna zastapic czytaniem z pliku i usuwaniem
	// Nastepnie dodac przekierowanie naszego outputu do pliku
	
	sem_post(mutex); // We leave the section
	sem_post(memory_is_full);
}

void writer()
{
	char result[SIZE_OF_MEMORY];
	sem_wait(memory_is_full); // We check wether we can write or not
	sem_wait(mutex);
	
	memcpy(our_memory, result, sizeof(result)); // Zastapic pisaniem do pliku
	
	sem_post(memory_is_empty);
	sem_post(mutex); // We leave the section
}

void archive()
{ // Jedyne co on bedzie robil to sprawdzal, czy nie zajelismy zbyt duzo miejsca i jesli tak to nadpisywal od przodu
}
void* sh_mem_creation(size_t s)
{
	return mmap(NULL, s, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS , -1, 0);
}
