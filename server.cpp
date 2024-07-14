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

void Server::removeClients(int clifd)
{
    delete _clients[clifd];
    _clients.erase(clifd);

    for (int i = 0; i < _pollFds.size(); i++)
    {
        if (_pollFds[i].fd == clifd)
        {
            _pollFds.erase(_pollFds.begin() + i);
            break;
        }
    }
}

void Server::recvData(int clifd)
{
    char buffer[1024];
    std::string message_buffer;

    int ret = recv(clifd, buffer, sizeof(buffer) - 1, 0); //-> receive data from the client
    if (ret == -1)
        throw(std::runtime_error("recv() faild"));
    else if (ret == 0) // 클라이언트에서 소켓 close() 호출, 정상 종료
    {
        removeClients(clifd);
        std::cout << RED << "client <" << clifd << "> disconnected" << WHI << std::endl;
        close(clifd);
    }
    else
    {
        buffer[ret] = '\0';
        std::cout << GRE << "Received message: " << buffer << WHI << std::endl;
        send(clifd, buffer, ret, 0);
        // message_buffer += buffer;

        // size_t pos;
        // while ((pos = message_buffer.find("\r\n")) != std::string::npos)
        // {
        //     std::string complete_message = message_buffer.substr(0, pos);
        //     message_buffer.erase(0, pos + 2);

        //     std::cout << GRE << "Received message: " << complete_message << WHI << std::endl;

        //     std::string response = "Message received: " + complete_message + "\r\n";
        //     send(clifd, response.c_str(), response.length(), 0);
        // }
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

void Server::serverInit()
{
    serverSocket();

    std::cout << GRE << "Server <" << _serverSocket << "> Connected" << WHI << std::endl;
    std::cout << "Waiting to accept a connection...\n";

    while (_signal == false)
    {
        if((poll(&_pollFds[0], _pollFds.size(),-1) == -1) && _signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));

        for (size_t i = 0; i < _pollFds.size(); i++) //-> check all file descriptors
		{
			if (_pollFds[i].revents & POLLIN)//-> check if there is data to read
			{
				if (_pollFds[i].fd == _serverSocket)
					addClient(); //서버 소켓에서 입력이 들어왔으므로 새 클라이언트 추가
				else
					recvData(_pollFds[i].fd); //클라이언트 소켓에서 입력이 들어왔으므로 데이터 receive
				//클라이언트를 통해 받은 데이터를 통해 서버가 데이터를
			}
		}
    }
}

