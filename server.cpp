#include "server.hpp"
#include "client.hpp"

bool Server::_signal = false;

Server::Server() : _serverSocket(-1) {}

void Server::signalHandler(int signal)
{
    (void)signal;
    std::cout << RED << "Signal received" << std::endl;
    _signal = true;
}

void Server::closeFds()
{
    // for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    // {
    //     close(it->first);
    // }
    // std::cout << RED << "Server <" << _serverSocket << "> Disconnected" << WHI << std::endl;
    // close(_serverSocket);
}


Client* Server::getClient(int clientFd)
{
    return (_clients[clientFd]);
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

    cli->setfd(new_fd);
    cli->setipadd(inet_ntoa(add.sin_addr));
    _clients[new_fd] = cli;
    
    std::cout << GRE << "Client <" << new_fd << "> Connected" << WHI << std::endl;
}

void Server::serverSocket()
{
    struct sockaddr_in add;
    struct pollfd new_poll;

    add.sin_family = AF_INET;
    add.sin_port = htons(SERVER_PORT);
    add.sin_addr.s_addr = INADDR_ANY;

    _serverSocket = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
    if (_serverSocket == -1)
        throw(std::runtime_error("faild to create socket"));
    int en = 1;
    if(setsockopt(_serverSocket, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_serverSocket, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(_serverSocket, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_serverSocket, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

    new_poll.fd = _serverSocket; //-> add the server socket to the pollfd
	new_poll.events = POLLIN; //-> set the event to POLLIN for reading data
	new_poll.revents = 0; //-> set the revents to 0
	_pollFds.push_back(new_poll); //-> add the server socket to the pollfd
}








// 문자열 벡터 params를 반환
std::vector<std::string> Server::makeParams(std::string completeMessage)
{
    std::vector<std::string> params;
    std::string tmp = "";

    if (completeMessage.size() == 0 || completeMessage[0] != ' ')
        throw (std::runtime_error("클라이언트 메세지 형식 err"));
    
    for (int i = 0; i < completeMessage.size(); i++)
    {
        if (completeMessage[i] == ' ')
            continue ;
        else if (completeMessage[i] == ':')
        {
            tmp = completeMessage.substr(i);
            params.push_back(tmp);
            return (params);
        }
        else
        {
            while (i < completeMessage.size() && completeMessage[i] != ' ')
                tmp += completeMessage[i++];
            params.push_back(tmp);
            tmp.clear();
        }
    }
    return (params);
}

std::string Server::makeCommand(std::string& completeMessage)
{
    std::string command = "";
    int i = 0;

    while (i != completeMessage.size())
    {
        if (completeMessage[i] == ' ')
            break ;
        command += completeMessage[i];
        i++;
    }
    completeMessage = completeMessage.substr(i, completeMessage.length() - i);
    return (command);
}

// completemessage에서 prefix를 추출해서 반환, completemessage는 prefix를 제외하고 저장됨
std::string Server::makePrefix(std::string& completeMessage)
{
    std::string prefix = "";
    int i = 0;

    if (completeMessage[0] != ':')
        prefix = "";
    else
    {
        while (i != completeMessage.size())
        {
            prefix += completeMessage[i]; 
            i++;
            if (completeMessage[i] != ' ' && completeMessage[i - 1] == ' ')
                break ;
        }
        completeMessage = completeMessage.substr(i, completeMessage.length() - i);
    }
    return (prefix);
}

// client의 버퍼와 현재 recv한 메세지를 combine해서 클라이언트 객체에 저장, 반환
std::string Server::makeCombinedMessage(std::string clientMessage, int clientFd)
{
    std::string remainMessage = getClient(clientFd)->popMessageBuff();
    remainMessage += clientMessage;
    return (remainMessage);
}

// server에서 특정 client 삭제
void Server::removeClient(int clientFd)
{
    std::cout << "disconnect [" << clientFd << "] Client" << "\n";

    delete _clients[clientFd];
    _clients.erase(clientFd);

    for (int i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == clientFd)
        {
            _pollFds.erase(_pollFds.begin() + i);
            break ;
        }
    }
}

// target client 소켓에서 메세지 recv
std::string Server::recvClientMessage(int clientFd)
{
    char    buff[512];
    int     readLen;
    std::string receivedMessage = "";

    while (1)
    {
        readLen = recv(clientFd, buff, sizeof(buff), 0);
        if (readLen < 0)
        {
            if (errno == EAGAIN) //recv 소켓의 데이터를 모두 읽었음을 의미
                break ;
            else
                throw(std::runtime_error("recv() faild"));
        }
        else if (readLen == 0) // 반환값 '0'인 경우 client disconnect를 의미
            return ("");
        else
        {
            std::string tmp(buff, readLen);
            receivedMessage += tmp;
        }
    }
    return (receivedMessage);
}

// 본격적인 파싱, combined 메세지에서 irc프로토콜의 메세지를 분리 후 파싱, 남은건 client 버퍼에 저장
void Server::handleCombinedMessage(std::string combinedMessage, int clientFd)
{
    std::string message;
    
    for (int i = 0; i < combinedMessage.size(); i++)
    {
        message += combinedMessage[i];
        if (1 < message.size() && message[message.size() - 1] == '\n' && message[message.size() - 2] == '\r')
        {
            std::string completeMessage = message.substr(0, message.size() - 2);

            std::string prefix = makePrefix(completeMessage);
            std::string command = makeCommand(completeMessage);
            std::vector<std::string> params = makeParams(completeMessage);
            //파싱된 데이터를 이용한 로직 구현부분 .. 일단 파싱 데이터를 하나로 묶는 클래스 ?

        
            message.clear();
        }
    }
    // cr-lf가 없는 message는 client 객체에 저장
    getClient(clientFd)->pushMessageBuff(message);

    // cr-lf가 입력되지 않아서 클라이언트 객체에 저장된 msg를 pop하는 테스트
    // std::cout << "클라이언트 임시 버퍼 pop! :" << _clients[clientFd]->popMessageBuff() << std::endl;

    return ;
}

// client request 처리
void Server::handleClientRequest(int targetFd)
{
    std::cout << "handle client request...\n";
    std::string clientMessage = recvClientMessage(targetFd); // target 소켓에서 데이터 recv

    if (clientMessage.size() == 0) // 소켓 이벤트 POLLIN의 메세지가 "" 빈문자열 메세지일 경우 disconnect를 의미
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
    for (int i = 0; i < _pollFds.size(); i++)
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
        if((poll(&_pollFds[0], _pollFds.size(), -1) == -1) && _signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));
        handleEvent();
    }
}

