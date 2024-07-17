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
        Client* getClient(int clientFd);
        void removeClient(int clientFd);

        void closeFds();

        // massage parsing
        void handleEvent();
        void handleClientRequest(int targetFd);
        std::string recvClientMessage(int clientFd);
        std::string makeCombinedMessage(std::string clientMessage, int clientFd);
        void handleCombinedMessage(std::string combinedMessage, int clientFd);

        std::string makePrefix(std::string& completeMessage);
        std::string makeCommand(std::string& completemessage);
        std::vector<std::string> makeParams(std::string completeMessage);
};


#endif