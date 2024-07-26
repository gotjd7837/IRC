#include "Server/Server.hpp"

int main(int ac, char **av)
{
	if (ac != 3)
	{
		std::cerr << "Usage: ./ircserv [port] [password]" << std::endl;
		return (1);
	}
	Server ser(av[2], av[1]);
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::signalHandler);
		signal(SIGQUIT, Server::signalHandler);
		ser.serverInit();
	}
	catch(const std::exception& e){
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}