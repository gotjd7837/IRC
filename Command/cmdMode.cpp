#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdMode(MessageProtocol& parsedMessage, int clientFd)
{
    Client* client = getClient(clientFd);
    std::string channelName = parsedMessage.getParams()[0];
    Channel* channel = getChannel(channelName);
    if (parsedMessage.getParams().size() < 2 || channel == nullptr)
        return ;
    if (channel->isOper(client) == false)
    {
        ucastMsg(clientFd, std::string("482 " + channelName + " :You're not channel operator"));
        return ;
    }

    std::string mode = parsedMessage.getParams()[1];
    if (mode == "+i")
    {
        channel->addMode(MODE_I);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else if (mode == "-i")
    {
        channel->removeMode(MODE_I);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else if (mode == "+t")
    {
        channel->addMode(MODE_T);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else if (mode == "-t")
    {
        channel->removeMode(MODE_T);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else if (mode == "+o")
    {
        if (parsedMessage.getParams().size() < 3)
        {
            ucastMsg(clientFd, std::string("461 " + client->getNick() + " MODE :Not enough parameters"));
            return ;
        }
        std::string nick = parsedMessage.getParams()[2];
        Client* target = channel->getMember(nick);
        if (target == nullptr)
        {
            ucastMsg(clientFd, std::string("401 " + client->getNick() + " " + nick + " :No such nick/channel"));
            return ;
        }
        channel->addOper(target);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode + " " + nick));
    }
    else if (mode == "-o")
    {
        if (parsedMessage.getParams().size() < 3)
        {
            ucastMsg(clientFd, std::string("461 " + client->getNick() + " MODE :Not enough parameters"));
            return ;
        }
        std::string nick = parsedMessage.getParams()[2];
        Client* target = channel->getMember(nick);
        if (target == nullptr)
        {
            ucastMsg(clientFd, std::string("401 " + client->getNick() + " " + nick + " :No such nick/channel"));
            return ;
        }
        channel->removeOper(target);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode + " " + nick));
    }
    else if (mode == "+k")
    {
        if (parsedMessage.getParams().size() < 3)
        {
            ucastMsg(clientFd, std::string("461 " + client->getNick() + " MODE :Not enough parameters"));
            return ;
        }
        std::string key = parsedMessage.getParams()[2];
        channel->setKey(key);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode + " " + key));
    }
    else if (mode == "-k")
    {
        channel->setKey("");
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else if (mode == "+l")
    {
        if (parsedMessage.getParams().size() < 3)
        {
            ucastMsg(clientFd, std::string("461 " + client->getNick() + " MODE :Not enough parameters"));
            return ;
        }
        std::string limit = parsedMessage.getParams()[2];
        int limitInt = std::stoi(limit);
        if (limitInt < 0)
        {
            ucastMsg(clientFd, std::string("472 " + client->getNick() + " " + limit + " :Channel limit must be greater than 0"));
            return ;
        }
        channel->setLimit(limitInt);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode + " " + limit));
    }
    else if (mode == "-l")
    {
        channel->setLimit(UINT_MAX);
        ccastMsg(channelName, std::string(client->getPrefix() + " MODE " + channelName + " " + mode));
    }
    else
        ucastMsg(clientFd, std::string("472 " + client->getNick() + " " + mode + " :is unknown mode char to me"));
}
