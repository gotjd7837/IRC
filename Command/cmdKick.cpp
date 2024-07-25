#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdKick(MessageProtocol& parsedMessage, int clientFd)
{
    if (parsedMessage.getParams().size() < 2)
    {
        ucastMsg(clientFd, std::string("461 " + getClient(clientFd)->getNick() + " KICK :Not enough parameters"));
        return ;
    }

    std::string channelName = parsedMessage.getParams()[0];
    Client* client = getClient(clientFd);
    Channel* channel = getChannel(channelName);
    if (channel == nullptr)
    {
        ucastMsg(clientFd, std::string("403 " + client->getNick() + " " + channelName + " :No such channel"));
        return ;
    }
    if (channel->isOper(client) == false)
    {
        ucastMsg(clientFd, std::string("482 " + channelName + " :You're not channel operator"));
        return ;
    }

    std::string targetNick = parsedMessage.getParams()[1];
    Client* targetClient = channel->getMember(targetNick);
    if (targetClient == nullptr)
    {
        ucastMsg(clientFd, std::string("441 " + client->getNick() + " " + targetNick + " :They aren't on that channel"));
        return ;
    }

    ccastMsg(channelName, std::string(client->getPrefix() + " KICK " + channelName + " " + targetNick + " " + parsedMessage.getParams()[2]));
    channel->removeMember(targetClient);
    if (channel->getMembers().empty())
        removeChannel(channelName);
}