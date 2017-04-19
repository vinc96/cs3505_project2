server_start: main.cpp server.o
	g++ main.cpp server.o logger.o socket_manager.o -o server_start \
	-std=c++11 -Wl,-rpath,"/usr/local/lib" -lboost_system
server.o: server.h server.cpp socket_manager.o logger.o
	g++ -c server.cpp -std=c++11
socket_manager.o: socket_manager.h socket_manager.cpp  
	g++ -c socket_manager.cpp -std=c++11
logger.o: logger.h logger.cpp
	g++ -c logger.cpp -std=c++11
clean:
	rm *.o
