CXX = g++

all: DBAccess.o DBMessageAccess.o DBAchivesAccess.o Sentence.o Sentence1.o Sentence2.o Sentence3.o SentenceGenerator.o Messenger.o CensorshipProgram.o Tests.o test21.o main.o
	$(CXX) DBAccess.o DBMessageAccess.o DBAchivesAccess.o Sentence.o Sentence1.o Sentence2.o Sentence3.o SentenceGenerator.o Messenger.o CensorshipProgram.o Tests.o test21.o main.o -l sqlite3 -pthread -o test

DBAccess.o: DBAccess.cpp DBAccess.h
	$(CXX) DBAccess.cpp -c -o DBAccess.o
	
DBMessageAccess.o: DBMessageAccess.cpp DBMessageAccess.h
	$(CXX) DBMessageAccess.cpp -c -o DBMessageAccess.o
	
DBAchivesAccess.o: DBAchivesAccess.cpp DBAchivesAccess.h
	$(CXX) DBAchivesAccess.cpp -c -o DBAchivesAccess.o
	
Sentence.o: Sentence.cpp Sentence.h
	$(CXX) Sentence.cpp -c -o Sentence.o

Sentence1.o: Sentence1.cpp Sentence1.h Sentence.h
	$(CXX) Sentence1.cpp -c -o Sentence1.o

Sentence2.o: Sentence2.cpp Sentence2.h Sentence.h
	$(CXX) Sentence2.cpp -c -o Sentence2.o

Sentence3.o: Sentence3.cpp Sentence3.h Sentence.h
	$(CXX) Sentence3.cpp -c -o Sentence3.o
	
CensorshipProgram.o: CensorshipProgram.cpp CensorshipProgram.h DBAccess.h DBMessageAccess.h DBAchivesAccess.h
	$(CXX) CensorshipProgram.cpp -c -o CensorshipProgram.o

Messenger.o: Messenger.cpp Messenger.h DBAccess.h DBMessageAccess.h Sentence1.h Sentence2.h Sentence3.h
	$(CXX) Messenger.cpp -c -o Messenger.o

SentenceGenerator.o: SentenceGenerator.cpp SentenceGenerator.h Sentence1.h
	$(CXX) SentenceGenerator.cpp -c -o SentenceGenerator.o

Tests.o: Tests.cpp Tests.h DBAccess.h DBMessageAccess.h DBAchivesAccess.h Sentence.h Sentence1.h Sentence2.h Sentence3.h SentenceGenerator.h  Messenger.h CensorshipProgram.h
	$(CXX) Tests.cpp -c -l sqlite3 -o Tests.o
	
test21.o: test21.cpp test21.h DBAccess.h DBMessageAccess.h DBAchivesAccess.h Sentence.h Sentence1.h Sentence2.h Sentence3.h SentenceGenerator.h  Messenger.h CensorshipProgram.h
	$(CXX) test21.cpp -c -l sqlite3 -o test21.o
	
main.o: main.cpp SentenceGenerator.h Tests.h
	$(CXX) main.cpp -c -l sqlite3 -o main.o 

clean:
	rm -f *.o test
