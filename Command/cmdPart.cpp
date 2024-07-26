#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdPart(MessageProtocol& parsedMessage, int clientFd)
{
    std::string channelName = parsedMessage.getParams()[0];
    Client* client = getClient(clientFd);
    Channel* channel = getChannel(channelName);

    if (!client->isRegistered())
    {
        ucastMsg(clientFd, "451 PART :You have not registered");
        return ;
    }
    if (parsedMessage.getParams().empty())
    {
        ucastMsg(clientFd, std::string("461 " + client->getNick() + " PART :Not enough parameters"));
        return ;
    }
    if (channel == nullptr)
    {
        ucastMsg(clientFd, std::string("403 " + client->getNick() + " " + channelName + " :No such channel"));
        return ;
    }
    if (channel->isMember(client) == false)
    {
        ucastMsg(clientFd, std::string("442 " + client->getNick() + " " + channelName + " :You're not on that channel"));
        return ;
    }
    
    ccastMsg(channelName, std::string(client->getPrefix() + " PART " + channelName + " " + parsedMessage.getParams()[1]));
    ucastMsg(clientFd, std::string("442 " + client->getNick() + " " + channelName + " :You have left the channel"));
    channel->removeMember(client);
    if (channel->getMembers().empty())
        removeChannel(channelName);
}
