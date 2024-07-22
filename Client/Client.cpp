#include "Client.hpp"

Client::Client() : 
_fd(-1), 
_cert(false),
_ipaddr(""),
_nickname(""), 
_username(""), 
_realname(""), 
_password("")  {}

int Client::getFd()
{
    return (_fd);
}

bool Client::getCert()
{
    return (_cert);
}

std::string Client::getPass()
{
    return (_password);
}

std::string Client::getNick()
{
    return (_nickname);
}

std::string Client::getUser()
{
    return (_username);
}

void Client::setNick(std::string nick)
{
    _nickname = nick;
}

void Client::setUser(std::string user)
{
    _username = user;
}

void Client::setPass(std::string pass)
{
    _password = pass;
}

void Client::setCert(bool cert)
{
    _cert = cert;
}

void Client::setFd(int fd)
{
    _fd = fd;
}

void Client::setIpaddr(std::string ipaddr) 
{
    _ipaddr = ipaddr;
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

// void Client::reply(std::string message)
// {
//     std::string prefix = _nickname + "!" + _username + "@" + _ipadd + " ";
//     message = ":" + prefix + message + "\r\n";

//     if (send(_fd, message.c_str(), message.size(), 0) < 0)
//         throw(std::runtime_error("send() failed"));
// }