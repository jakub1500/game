all:
	g++ -lncurses -pthread game.cpp screen.cpp action.cpp world/field.cpp world/player.cpp world/world.cpp net/net_client.cpp net/net_common.cpp net/msg_processer.cpp net/net_session.cpp -o game
	g++ server.cpp net/net_server.cpp net/net_common.cpp world/world.cpp world/field.cpp world/player.cpp net/msg_processer.cpp net/net_session.cpp -pthread -o server
