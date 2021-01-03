all:
	g++ -lncurses -pthread game.cpp screen.cpp action.cpp -o game
	g++ server.cpp net/net_server.cpp net/net_common.cpp -pthread -o server
