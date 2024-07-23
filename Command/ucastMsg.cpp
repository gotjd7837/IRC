#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::ccastMsg(std::string channelName, std::string msg)
{
    msg += "\r\n";

    for (std::map<Client*, bool>::const_iterator it = _channels[channelName]->getMembers().begin(); it != _channels[channelName]->getMembers().end(); it++)
    {
        if (send(it->first->getFd(), msg.c_str(), msg.size(), 0) < 0)
            throw(std::runtime_error("send() failed"));
    }
}

void Server::ucastMsg(int clientFd, std::string msg)
{
    msg += "\r\n";

    if (send(clientFd, msg.c_str(), msg.size(), 0) < 0)
        throw(std::runtime_error("send() failed"));
    // send를 실패했다고 해서 서버 프로그램을 종료할 필요는 없을 것 같다.
}