#include "Client.hpp"

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

std::string Client::popMessageBuff()
{
    std::string remainMessage = _messageBuff;

    _messageBuff.clear();
    return (remainMessage);
}

void Client::pushMessageBuff(std::string& message)
{
    _messageBuff += message;
}