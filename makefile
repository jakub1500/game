all:
	g++ -lncurses -pthread game.cpp screen.cpp action.cpp world/field.cpp world/player.cpp world/world.cpp net/net_common.cpp net/msg_processer.cpp net/net_session.cpp net/net_agent.cpp -o game
	g++ -lncurses server.cpp net/net_common.cpp  screen.cpp world/world.cpp world/field.cpp world/player.cpp net/msg_processer.cpp net/net_session.cpp net/net_agent.cpp -pthread -o server
