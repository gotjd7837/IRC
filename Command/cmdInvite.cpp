#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

Client* Server::findClientByNick(std::string nick)
{
    for (std::map<int, Client *>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (it->second->getNick() == nick)
            return (it->second);
    }
    return (nullptr);
}

void Server::cmdInvite(MessageProtocol& parsedMessage, int clientFd)
{
    std::string channelName = parsedMessage.getParams()[1];
    std::string nickName = parsedMessage.getParams()[0];
    Channel* channel = getChannel(channelName);
    Client* client = getClient(clientFd);

    if (!client->isRegistered())
    {
        ucastMsg(clientFd, "451 INVITE :You have not registered");
        return ;
    }
    if (parsedMessage.getParams().size() < 2)
    {
        ucastMsg(clientFd, "461 " + channelName + " INVITE :Not enough parameters");
        return ;
    }
    if (channel == nullptr)
    {
        ucastMsg(clientFd, "403 " + channelName + " :No such channel");
        return ;
    }

    Client* targetClient = channel->getMember(nickName);
    if (targetClient == nullptr)
    {
        // 초대 프로세스
        targetClient = findClientByNick(nickName);
        if (targetClient == nullptr)
        {
            ucastMsg(clientFd, "401 " + nickName + " :No such nick/channel");
            return ;
        }

        channel->addInvite(nickName);
        // 초대 받은 사람에게 보내는 메세지
        ucastMsg(targetClient->getFd(), client->getPrefix() + " INVITE " + targetClient->getNick() + " " + channelName);
        
        // 초대한 사람에게 보내는 메세지
        ucastMsg(clientFd, client->getPrefix() + " 341 " + client->getNick() + " " + targetClient->getNick() + " " + channelName);
    }
    else
        ucastMsg(clientFd, "443 " + channelName + " " + nickName + " :is already a member");
}