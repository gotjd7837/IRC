#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::ccastMsg(std::string channelName, std::string msg)
{
    std::cout << GRE << "send to <" << channelName << "> : " << msg << WHI << std::endl;
    for (std::map<Client*, bool>::const_iterator it = _channels[channelName]->getMembers().begin(); it != _channels[channelName]->getMembers().end(); it++)
        ucastMsg(it->first->getFd(), msg);
}
