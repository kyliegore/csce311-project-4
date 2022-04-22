cc = g++

flags = -std=c++17
flags += -Wall
flags += -g



#
text-server: text-server.cc
	g++ -g -o text-server text-server.cc -lrt -lpthread


#
text-client: text-client.cc
	g++ -g -o text-client text-client.cc -lrt -lpthread


#
clean:
	$(RM) text-server text-client