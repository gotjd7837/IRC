#ifndef SERVER_HPP

#include <iostream>
#include <vector>
#include <map>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <poll.h>
#include <csignal>

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

#define SERVER_PORT 6667

class Client;

class Server
{
    private :
        int _serverSocket;
        static bool _signal;
        std::map<int, Client *> _clients;
        std::vector<struct pollfd> _pollFds;

    public :
        Server();

        static void signalHandler(int _signal);

        void serverInit();
        void serverSocket();

        void addClient();
        void recvData(int clifd);

        void closeFds();
        void removeClients(int clifd);
};


#endif