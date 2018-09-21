
all:
	g++ -std=c++11 -fPIC -Wall -g -c *.cpp
	g++ -o gbook *.o -lssl -lcrypto -lrt -lpthread -lsqlite3

clean:
	rm -rf *.o

superclean: clean
	rm -rf *.a

supermake: superclean all
