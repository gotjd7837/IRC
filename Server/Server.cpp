#include "Server.hpp"
#include "Client.hpp"
#include "MessageProtocol.hpp"

bool Server::_signal = false;

Server::Server() : _serverSocket(-1) {}

Server::Server(std::string port, std::string password) : _password(password), _port(port), _serverSocket(-1) {}

Server::~Server()
{
    //메모리 해제
}

void Server::signalHandler(int signal)
{
    (void)signal;
    std::cout << RED << "Signal received" << std::endl;
    _signal = true;
}

Client* Server::getClient(int clientFd)
{
    return (_clients[clientFd]);
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








// command 처리 인터페이스
void Server::excuteCommand(MessageProtocol parsedMessage, int clientFd)
{
    std::cout << "prefix : " << parsedMessage.getPrefix() << std::endl;
    std::cout << "command : " << parsedMessage.getCommand() << std::endl;
    for (size_t i = 0; i < parsedMessage.getParams().size(); i++)
        std::cout << "params : " << parsedMessage.getParams()[i] << std::endl;


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

/*
PASS : 서버에 연결하기 위한 패스워드를 확인한다.
NICK : 닉네임을 변경한다.
USER : 유저 이름을 변경한다.
JOIN : 채널에 입장한다.
PART : 채널에서 나간다.
PRIVMSG : 특정 사용자 또는 채널에 메시지를 보낸다.
NOTICE : 서버의 유저에게 메시지를 보낸다.
LIST : 현재 서버에서 사용 가능한 채널 목록을 조회한다.
PING : 클라이언트-서버 간의 연결을 확인한다.
OPER : 관리자 권한을 얻는다.
KICK : 유저를 특정 채널에서 내보낸다.
QUIT : IRC 서버에서 나간다.
*/




// client의 버퍼와 현재 recv한 메세지를 combine해서 클라이언트 객체에 저장, 반환
std::string Server::makeCombinedMessage(std::string clientMessage, int clientFd)
{
    std::string remainMessage = getClient(clientFd)->popMessageBuff();
    remainMessage += clientMessage;
    return (remainMessage);
}

// target client 소켓에서 메세지 recv
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

// 본격적인 파싱, combined 메세지에서 irc프로토콜의 메세지를 분리 후 파싱, 남은건 client 버퍼에 저장
void Server::handleCombinedMessage(std::string combinedMessage, int clientFd)
{
    std::string tmp;
    
    for (int i = 0; i < combinedMessage.size(); i++)
    {
        tmp += combinedMessage[i];
        if (1 < tmp.size() && tmp[tmp.size() - 1] == '\n' && tmp[tmp.size() - 2] == '\r') // cr-lf가 있는 완전한 메세지는 excuteCommand로 전달
        {
            std::string completeMessage = tmp.substr(0, tmp.size() - 2);
            excuteCommand(MessageProtocol(completeMessage), clientFd);
            tmp.clear();
        }
    }
    // cr-lf가 없는 message는 client 객체 _messageBuff에 저장
    getClient(clientFd)->pushMessageBuff(tmp);
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

