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
#include <cctype>
#include <csignal>

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

#define SERVER_PORT 6667

class Client;
class Channel;
class MessageProtocol;

class Server
{
    private :
        std::string _name;
        
        int _serverSocket;
        static bool _signal;
        std::string _password;
        std::string _port;

        std::map<std::string, Channel *> _channels;
        std::map<int, Client *> _clients;
        std::vector<struct pollfd> _pollFds;

    public :
        Server();
        Server(std::string password, std::string port);
        ~Server();

        static void signalHandler(int _signal);

        void serverInit();
        void serverSocket();

        void addClient();
        Client* getClient(int clientFd);
        void removeClient(int clientFd);

        Channel* getChannel(std::string channelName);
        void removeChannel(std::string channelName);
        void addChannel(std::string channelName);

        void handleEvent();
        void handleClientRequest(int targetFd);
        std::string recvClientMessage(int clientFd);
        std::string makeCombinedMessage(std::string clientMessage, int clientFd);
        void handleCombinedMessage(std::string combinedMessage, int clientFd);

        // command
        void excuteCommand(MessageProtocol parsedMessage, int clientFd);
        void cmdPass(MessageProtocol& parsedMessage, int clientFd);
        void cmdUser(MessageProtocol& parsedMessage, int clientFd);
        void cmdNick(MessageProtocol& parsedMessage, int clientFd);
        void cmdPong(MessageProtocol& parsedMessage, int clientFd);
        void cmdJoin(MessageProtocol& parsedMessage, int clientFd);
        void cmdPart(MessageProtocol& parsedMessage, int clientFd);
        void cmdKick(MessageProtocol& parsedMessage, int clientFd);
        void cmdPrivMsg(MessageProtocol& parsedMessage, int clientFd);
        void cmdMode(MessageProtocol& parsedMessage, int clientFd);
        void cmdQuit(MessageProtocol& parsedMessage, int clientFd);
        void cmdTopic(MessageProtocol& parsedMessage, int clientFd);
        void cmdInvite(MessageProtocol& parsedMessage, int clientFd);

        Client* findClientByNick(std::string nick);

        void clientCert(int clientFd);

        void bcastMsg(std::string msg);
        void ucastMsg(int clientFd, std::string msg);
        void ccastMsg(std::string channelName, std::string msg);
};


#endif