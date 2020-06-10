CXX = g++

all: Sentence.o Sentence1.o SentenceGenerator.o Tests.o main.o
	$(CXX) Sentence.o Sentence1.o SentenceGenerator.o Tests.o main.o -o test

Sentence.o: Sentence.cpp Sentence.h
	$(CXX) Sentence.cpp -c -o Sentence.o

Sentence1.o: Sentence1.cpp Sentence1.h Sentence.h
	$(CXX) Sentence1.cpp -c -o Sentence1.o

SentenceGenerator.o: SentenceGenerator.cpp SentenceGenerator.h Sentence1.h
	$(CXX) SentenceGenerator.cpp -c -o SentenceGenerator.o

Tests.o: Tests.cpp Tests.h
	$(CXX) Tests.cpp -c -o Tests.o
	
main.o: main.cpp SentenceGenerator.h Tests.h
	$(CXX) main.cpp -c -o main.o 

clean:
	rm -f *.o test
