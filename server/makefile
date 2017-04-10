server_start: main.cpp server.o
	g++ main.cpp server.o -o server_start
server.o: server.h server.cpp socket_manager.o
	g++ -c server.cpp socket_manager.o
socket_manager.o: socket_manager.h socket_manager.cpp
	g++ -c socket_manager.cpp

clean:
	rm *.o
