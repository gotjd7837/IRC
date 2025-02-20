#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

bool validCheckNick(std::string nick)
{
    for (unsigned long i = 0; i < nick.size(); i++)
    {
        if (isdigit(nick[i]) || isalpha(nick[i]) || nick[i] == '_' || nick[i] == '-')
            continue ;
        else
            return (false);
    }
    return (true);
}

void Server::cmdNick(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    if (cli == nullptr)
        return ;
    
    if (!cli->getCert())
        clientCert(clientFd);
    if (!cli->getCert())
        return ;
    if (!validCheckNick(parsedMessage.getParams()[0]) || parsedMessage.getParams().size() > 1)
    {
        ucastMsg(clientFd, "432 " + parsedMessage.getParams()[0] + " :Erroneus nickname");
        return ;
    }

    std::string oldNick = cli->getNick();

    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        std::string nickname = it->second->getNick();
        if (nickname == parsedMessage.getParams()[0])
        {
            ucastMsg(clientFd, "407 " + parsedMessage.getParams()[0] + " :Nickname is already in use");
            return ;
        }
    }
    cli->setNick(parsedMessage.getParams()[0]);

    if (oldNick != "")
        bcastMsg(":" + oldNick + " NICK " + ":" + parsedMessage.getParams()[0]);
}