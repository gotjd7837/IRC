#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdTopic(MessageProtocol& parsedMessage, int clientFd)
{
    Client* client = getClient(clientFd);
    std::string channelName = parsedMessage.getParams()[0];
    Channel* channel = getChannel(channelName);
    if (channel == nullptr)
    {
        ucastMsg(clientFd, "403 " + channelName + " :No such channel");
        return;
    }

    if (parsedMessage.getParams().size() == 1)
    {
        if (channel->getTopic().empty())
            ucastMsg(clientFd, "331 " + channelName + " :No topic is set");
        else
            ucastMsg(clientFd, "332 " + channelName + " " + channel->getTopic());
    }
    else
    {
        if (channel->isMember(client))
        {
            if (channel->isOper(client) || !channel->hasMode(MODE_T))
            {
                channel->setTopic(parsedMessage.getParams()[1]);
                ccastMsg(channelName, client->getPrefix() + " TOPIC " + channelName + " " + parsedMessage.getParams()[1]);
            }
            else
                ucastMsg(clientFd, "482 " + channelName + " :You're not channel operator");
        }
        else
            ucastMsg(clientFd, "442 " + channelName + " :You're not on that channel");
    }

}