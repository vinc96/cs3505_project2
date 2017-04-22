server_start: main.cpp server.o
	g++ main.cpp server.o logger.o message_parser.o socket_manager.o \
	spreadsheet_pool.o spreadsheet_controller.o sqlite.o \
	-o server_start -std=c++11 -Wl,-rpath,"/usr/local/lib" -lboost_system -lpthread -ldl -g
server.o: server.h server.cpp message_parser.o socket_manager.o logger.o spreadsheet_controller.o
	g++ -c server.cpp -std=c++11 -g
socket_manager.o: socket_manager.h socket_manager.cpp  
	g++ -c socket_manager.cpp -std=c++11 -g
spreadsheet_controller.o: spreadsheet_controller.h spreadsheet_controller.cpp spreadsheet_pool.o
	g++ -c spreadsheet_controller.cpp -std=c++11 -g
spreadsheet_pool.o: spreadsheet_pool.cpp spreadsheet_pool.h sqlite.o
	g++ -c spreadsheet_pool.cpp -std=c++11 -g
sqlite.o: sqlite3.c
	gcc -c sqlite3.c -o sqlite.o
message_parser.o: message_parser.h message_parser.cpp
	g++ -c message_parser.cpp -std=c++11 -g
logger.o: logger.h logger.cpp
	g++ -c logger.cpp -std=c++11 -g
clean:
	rm -f *.o *~ server_start

