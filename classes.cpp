#include <iostream>
#include <vector>
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

class Client
{
    private :
        int _fd;
        std::string _ipadd;
    
    public :
        Client(){};
        int getfd(){return _fd;}

        void setfd(int fd){_fd = fd;}
        void setipadd(std::string ipadd){_ipadd = ipadd;}
};

class Server
{
    private :
        int _port;
        int _ser_socket_fd;
        static bool _signal;
        std::vector<Client> _clients;
        std::vector<struct pollfd> _fds;
    
    public :
        Server(){_ser_socket_fd = -1;}

        void server_init();
        void server_socket();
        void accept_new_client();
        void receive_new_data(int fd);

        static void _signal_handler(int _signal);

        void close_fds();
        void clear_clients(int fd);
};

void Server::clear_clients(int fd)
{
    for (int i = 0; i < _fds.size(); i++)
    {
        if (_fds[i].fd == fd)
        {
            _fds.erase(_fds.begin() + i);
            break ;
        }
    }
    for (int i = 0; i < _clients.size(); i++)
    {
        if (_clients[i].getfd() == fd)
        {
            _clients.erase(_clients.begin() + i);
            break ;
        }
    }
}

bool Server::_signal = false;

void Server::_signal_handler(int _signal)
{
    (void)_signal;
    std::cout << std::endl << "_signal received" << std::endl;
    Server::_signal = true;
}

void Server::close_fds()
{
    for (int i = 0; i < _clients.size(); i ++)
    {
        std::cout << RED << "client <" << _clients[i].getfd() << "> disconnected" << WHI << std::endl;
        close(_clients[i].getfd());
    }
    if (_ser_socket_fd != -1)
    {
        std::cout << RED << "Server <" << _ser_socket_fd << "> Disconnected" << WHI << std::endl;
        close(_ser_socket_fd);
    }
}
void Server::server_socket()
{
    struct sockaddr_in add;
    struct pollfd new_poll;

    add.sin_family = AF_INET;
    add.sin_port = htons(this->_port);
    add.sin_addr.s_addr = INADDR_ANY;

    _ser_socket_fd = socket(AF_INET, SOCK_STREAM, 0); //-> create the server socket
    if (_ser_socket_fd == -1)
        throw(std::runtime_error("faild to create socket"));
    int en = 1;
    if(setsockopt(_ser_socket_fd, SOL_SOCKET, SO_REUSEADDR, &en, sizeof(en)) == -1) //-> set the socket option (SO_REUSEADDR) to reuse the address
		throw(std::runtime_error("faild to set option (SO_REUSEADDR) on socket"));
    if (fcntl(_ser_socket_fd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		throw(std::runtime_error("faild to set option (O_NONBLOCK) on socket"));
    if (bind(_ser_socket_fd, (struct sockaddr *)&add, sizeof(add)) == -1) //-> bind the socket to the address
		throw(std::runtime_error("faild to bind socket"));
	if (listen(_ser_socket_fd, SOMAXCONN) == -1) //-> listen for incoming connections and making the socket a passive socket
		throw(std::runtime_error("listen() faild"));

    new_poll.fd = _ser_socket_fd; //-> add the server socket to the pollfd
	new_poll.events = POLLIN; //-> set the event to POLLIN for reading data
	new_poll.revents = 0; //-> set the revents to 0
	_fds.push_back(new_poll); //-> add the server socket to the pollfd
}

void Server::accept_new_client()
{
	Client cli; //-> create a new client
	struct sockaddr_in cliadd; // 소켓의 ip주소와 port를 저장하는 구조체
	struct pollfd new_poll;
	socklen_t len = sizeof(cliadd);

     //연결 요청이 수신될 때까지 블록되며 연결이 수신되면 클라이언트와 연결되는 소켓 fd를 반환하고 sockaddr_in 구조체에 정보 저장
	int incofd = accept(_ser_socket_fd, (sockaddr *)&(cliadd), &len);
	if (incofd == -1)
		{std::cout << "accept() failed" << std::endl; return;}

	if (fcntl(incofd, F_SETFL, O_NONBLOCK) == -1) //-> set the socket option (O_NONBLOCK) for non-blocking socket
		{std::cout << "fcntl() failed" << std::endl; return;}

	new_poll.fd = incofd; //-> add the client socket to the pollfd
	new_poll.events = POLLIN; //-> set the event to POLLIN for reading data
	new_poll.revents = 0; //-> set the revents to 0

	cli.setfd(incofd); //-> set the client file descriptor
	cli.setipadd(inet_ntoa((cliadd.sin_addr))); //-> convert the ip address to string and set it
	_clients.push_back(cli); //-> add the client to the vector of clients
	_fds.push_back(new_poll); //-> add the client socket to the pollfd

	std::cout << GRE << "Client <" << incofd << "> Connected" << WHI << std::endl;
}

void Server::receive_new_data(int fd)
{
	char buff[1024]; //-> buffer for the received data
	memset(buff, 0, sizeof(buff)); //-> clear the buffer

    //소켓 프로그래밍에서 연결된 소켓에서 데이터를 수신하는데 사용
	ssize_t bytes = recv(fd, buff, sizeof(buff) - 1 , 0); //데이터를 receive하는 함수, 읽은 바이트 크기를 반환

	if(bytes <= 0){ //클라이언트가 연결을 끊었거나 에러 발생한 상황
		std::cout << RED << "Client <" << fd << "> Disconnected" << WHI << std::endl;
		clear_clients(fd); //-> clear the client
		close(fd); //-> close the client socket
	}
	else{ //-> print the received data
		buff[bytes] = '\0';
		std::cout << YEL << "Client <" << fd << "> Data: " << WHI << buff;
		//here you can add your code to process the received data: parse, check, authenticate, handle the command, etc...
	}
}

void Server::server_init()
{
	this->_port = 4444;
	server_socket(); //-> create the server socket

	std::cout << GRE << "Server <" << _ser_socket_fd << "> Connected" << WHI << std::endl;
	std::cout << "Waiting to accept a connection...\n";
    
    while (Server::_signal == false) //-> run the server until the _signal is received
	{
		if((poll(&_fds[0], _fds.size(),-1) == -1) && Server::_signal == false) //-> wait for an event
			throw(std::runtime_error("poll() faild"));

		for (size_t i = 0; i < _fds.size(); i++) //-> check all file descriptors
		{
			if (_fds[i].revents & POLLIN)//-> check if there is data to read
			{
				if (_fds[i].fd == _ser_socket_fd)
					accept_new_client(); //서버 소켓에서 입력이 들어왔으므로 새 클라이언트 추가
				else
					receive_new_data(_fds[i].fd); //클라이언트 소켓에서 입력이 들어왔으므로 데이터 receive
			}
		}
	}
	close_fds(); //-> close the file descriptors when the server stops
}

int main()
{
	Server ser;
	std::cout << "---- SERVER ----" << std::endl;
	try{
		signal(SIGINT, Server::_signal_handler); //-> catch the _signal (ctrl + c)
		signal(SIGQUIT, Server::_signal_handler); //-> catch the _signal (ctrl + \)
		ser.server_init(); //-> initialize the server
	}
	catch(const std::exception& e){
		ser.close_fds(); //-> close the file descriptors
		std::cerr << e.what() << std::endl;
	}
	std::cout << "The Server Closed!" << std::endl;
}

/*
서버 객체와 클라이언트 객체 클래스를 정의한다
서버 객체는 멤버 변수로 클라이언트들의 객체를 가지고 있다.

irc 서버이고, fd가 블로킹 되지 않아야하기 때문에 vector<struct pollfd>를 정의한다.
struct pollfd 구조체는 "소켓 fd, 요청 이벤트" 의 정보를 저장한다.
pollfd 벡터는 서버, 클라이언트의 수 만큼의 크기를 가진다.

pollfd를 이용해서 poll()시스템 콜을 실행하면 한번에 하나씩의 fd i/o를 처리하지 않고
즉, 블로킹하지 않고 버퍼에 담긴 fd중 요청 이벤트가 있으면 i/o처리를 수행 하도록 한다.
poll()을 사용하면 이벤트가 발생할 때까지 호출 프로세스를 차단하여
바쁜 대기나 폴링의 필요성을 피하고 CPU 사용량을 최소화합니다.

*/