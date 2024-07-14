#include "client.hpp"

Client::Client() : _fd(-1) {}

int Client::getfd()
{
    return _fd;
}

void Client::setfd(int fd)
{
    _fd = fd;
}

void Client::setipadd(std::string ipadd) 
{
    _ipadd = ipadd;
}
