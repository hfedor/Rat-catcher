CXX = g++

all: DBAccess.o Sentence.o Sentence1.o Sentence2.o Sentence3.o SentenceGenerator.o Messenger.o CensorshipProgram.o Tests.o main.o
	$(CXX) DBAccess.o Sentence.o Sentence1.o Sentence2.o Sentence3.o SentenceGenerator.o Messenger.o CensorshipProgram.o Tests.o main.o -l sqlite3 -o test

DBAccess.o: DBAccess.cpp DBAccess.h
	$(CXX) DBAccess.cpp -c -o DBAccess.o
	
Sentence.o: Sentence.cpp Sentence.h
	$(CXX) Sentence.cpp -c -o Sentence.o

Sentence1.o: Sentence1.cpp Sentence1.h Sentence.h
	$(CXX) Sentence1.cpp -c -o Sentence1.o

Sentence2.o: Sentence2.cpp Sentence2.h Sentence.h
	$(CXX) Sentence2.cpp -c -o Sentence2.o

Sentence3.o: Sentence3.cpp Sentence3.h Sentence.h
	$(CXX) Sentence3.cpp -c -o Sentence3.o
	
CensorshipProgram.o: CensorshipProgram.cpp CensorshipProgram.h DBAccess.h
	$(CXX) CensorshipProgram.cpp -c -o CensorshipProgram.o

Messenger.o: Messenger.cpp Messenger.h DBAccess.h Sentence1.h Sentence2.h Sentence3.h
	$(CXX) Messenger.cpp -c -o Messenger.o
	
processes.o: processes.cpp 
	$(CXX) processes.cpp -c -o processes.o
	
SentenceGenerator.o: SentenceGenerator.cpp SentenceGenerator.h Sentence1.h
	$(CXX) SentenceGenerator.cpp -c -o SentenceGenerator.o

Tests.o: Tests.cpp Tests.h DBAccess.h Sentence.h Sentence1.h Sentence2.h Sentence3.h SentenceGenerator.h  Messenger.h CensorshipProgram.h
	$(CXX) Tests.cpp -c -l sqlite3 -o Tests.o
	
main.o: main.cpp SentenceGenerator.h Tests.h
	$(CXX) main.cpp -c -l sqlite3 -o main.o 

clean:
	rm -f *.o test
