#include "Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

bool Server::_signal = false;

Server::Server() : _serverSocket(-1) {}

Server::Server(std::string port, std::string password) : _serverSocket(-1), _password(password), _port(port) {}

Server::~Server()
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
        delete it->second;
    for (std::map<std::string, Channel *>::iterator it = _channels.begin(); it != _channels.end(); it++)
        delete it->second;
    if (_serverSocket != -1)
        close(_serverSocket);
}

void Server::signalHandler(int signal)
{
    (void)signal;
    std::cout << RED << "Signal received" << std::endl;
    _signal = true;
}

Client* Server::getClient(int clientFd)
{
    if (_clients.find(clientFd) == _clients.end())
        return (nullptr);
    return (_clients[clientFd]);
}

// server에서 특정 client 삭제
void Server::removeClient(int clientFd)
{
    std::cout << "disconnect [" << clientFd << "] Client" << "\n";

    // for (channel)
    //     {
    //         channaliter->removemember(_clients[clientFd])
    //     }
    delete _clients[clientFd];
    _clients.erase(clientFd);

    for (size_t i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == clientFd)
        {
            _pollFds.erase(_pollFds.begin() + i);
            break ;
        }
    }
    
    close(clientFd);
}

void Server::addClient()
{
    Client *cli = new Client();
    struct pollfd new_poll;

    struct sockaddr_in add;
    socklen_t len = sizeof(add);

    int new_fd = accept(_serverSocket, (struct sockaddr *)&add, &len); //-> accept the new connection
    if (new_fd == -1)
        throw(std::runtime_error("accept() faild"));

    if (fcntl(new_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
        throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));

    new_poll.fd = new_fd; //-> add the new client socket to the pollfd
    new_poll.events = POLLIN; //-> set the event to POLLIN for reading data
    new_poll.revents = 0; //-> set the revents to 0
    _pollFds.push_back(new_poll); //-> add the new client socket to the pollfd

    cli->setFd(new_fd);
    cli->setIpaddr(inet_ntoa(add.sin_addr));
    _clients[new_fd] = cli;
    
    std::cout << GRE << "Client <" << new_fd << "> Connected" << WHI << std::endl;
}

Channel* Server::getChannel(std::string channelName)
{
    if (_channels.find(channelName) == _channels.end())
        return (nullptr);
    return (_channels[channelName]);
}

void Server::removeChannel(std::string channelName)
{
    delete _channels[channelName];
    _channels.erase(channelName);
}

void Server::addChannel(std::string channelName)
{
    _channels[channelName] = new Channel(channelName);
}

void Server::serverSocket()
{
    struct sockaddr_in add;
    struct pollfd new_poll;

    add.sin_family = AF_INET;
    add.sin_port = htons(atoi(_port.c_str()));
    add.sin_addr.s_addr = INADDR_ANY;

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
    if (_serverSocket == -1)
        throw(std::runtime_error("faild to create socket"));
    int en = 1;
    if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1)
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1)
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(_serverSocket, (struct sockaddr *)&add, sizeof(add)) == -1)
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_serverSocket, SOMAXCONN) == -1)
		throw(std::runtime_error("listen() faild"));

    new_poll.fd = _serverSocket;
	new_poll.events = POLLIN;
	new_poll.revents = 0;
	_pollFds.push_back(new_poll);
}

void Server::excuteCommand(MessageProtocol parsedMessage, int clientFd)
{
    (void)clientFd;
    std::cout << "prefix : " << parsedMessage.getPrefix() << std::endl;
    std::cout << "command : " << parsedMessage.getCommand() << std::endl;
    for (size_t i = 0; i < parsedMessage.getParams().size(); i++)
        std::cout << "params : " << parsedMessage.getParams()[i] << std::endl;

    if (parsedMessage.getCommand() == "PASS")
        cmdPass(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "NICK")
        cmdNick(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "USER")
        cmdUser(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "PING")
        cmdPong(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "JOIN")
        cmdJoin(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "PRIVMSG")
        cmdPrivMsg(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "MODE")
        cmdMode(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "KICK")
        cmdKick(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "PART")
        cmdPart(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "TOPIC")
        cmdTopic(parsedMessage, clientFd);
    else if (parsedMessage.getCommand() == "INVITE")
        cmdInvite(parsedMessage, clientFd);
    else
        std::cout << "Unknown Command\n";

    // std::string cmd[] = {"INVITE", "JOIN", "KICK", "MODE", "NICK", "PART", "PASS", "PING", "PONG", "PRIVMSG", "QUIT", "TOPIC", "USER"};

    // void (Server::*func[13])(MessageProtocol, int) = {
        // &Server::cmdInvite,
        // &Server::cmdJoin,
        // &Server::cmdKick,
        // &Server::cmdMode,
        // &Server::cmdNick,
        // &Server::cmdPart,
        // &Server::cmdPass,
        // &Server::cmdPing,
        // &Server::cmdPong,
        // &Server::cmdPrivmsg,
        // &Server::cmdQuit,
        // &Server::cmdTopic,
        // &Server::cmdUser
    // };

    // for (int i = 0; i < 13; i++)
    // {
    //     if (parsedMessage.getCommand() == cmd[i])
    //     {
    //         (this->*func[i])(parsedMessage, clientFd);
    //         return ;
    //     }
    // }
}

std::string Server::makeCombinedMessage(std::string clientMessage, int clientFd)
{
    std::string remainMessage = getClient(clientFd)->popMessageBuff();
    remainMessage += clientMessage;
    return (remainMessage);
}

std::string Server::recvClientMessage(int clientFd)
{
    char    buff[512];
    int     readLen;
    std::string receivedMessage = "";

    readLen = recv(clientFd, buff, sizeof(buff), 0);
    if (readLen <= 0)
        return ("");
    else
    {
        std::string tmp(buff, readLen);
        receivedMessage += tmp;
    }
    return (receivedMessage);
}

void Server::handleCombinedMessage(std::string combinedMessage, int clientFd)
{
    std::string tmp;
    
    for (size_t i = 0; i < combinedMessage.size(); i++)
    {
        tmp += combinedMessage[i];
        if (1 < tmp.size() && tmp[tmp.size() - 1] == '\n' && tmp[tmp.size() - 2] == '\r')
        {
            std::string completeMessage = tmp.substr(0, tmp.size() - 2);
            excuteCommand(MessageProtocol(completeMessage), clientFd);
            tmp.clear();
        }
    }
    if (getClient(clientFd) != nullptr)
        getClient(clientFd)->pushMessageBuff(tmp);
    return ;
}

void Server::handleClientRequest(int targetFd)
{
    std::cout << YEL << "handle client request..." << WHI << std::endl;
    std::string clientMessage = recvClientMessage(targetFd);

    if (clientMessage.size() == 0)
    {
        removeClient(targetFd);
        return ;
    }

    std::string combinedMessage = makeCombinedMessage(clientMessage, targetFd);
    handleCombinedMessage(combinedMessage, targetFd);
    return ;
}

void Server::handleEvent()
{
    for (size_t i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].revents & POLLIN)
        {
            if (_pollFds[i].fd == _serverSocket)
                addClient();
            else
                handleClientRequest(_pollFds[i].fd);
        }
    }
}

void Server::serverInit()
{
    serverSocket();

    std::cout << GRE << "Server <" << _serverSocket << "> Connected" << WHI << std::endl;
    std::cout << "Waiting to accept a connection...\n";

    while (_signal == false)
    {
        if((poll(&_pollFds[0], _pollFds.size(), -1) == -1) && _signal == false)
			throw(std::runtime_error("poll() failed"));
        handleEvent();
    }
}

