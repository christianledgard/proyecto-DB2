all: sequentialFile

sequentialFile: main.cpp
	g++ -pthread -o main.out main.cpp -std=c++11
	./main.out
	rm -f main.out
clean:
	rm -f main.out