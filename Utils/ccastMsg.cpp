#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::ccastMsg(std::string channelName, std::string msg)
{
    msg += "\r\n";

    for (std::map<Client*, bool>::const_iterator it = _channels[channelName]->getMembers().begin(); it != _channels[channelName]->getMembers().end(); it++)
        send(it->first->getFd(), msg.c_str(), msg.size(), 0);
}
