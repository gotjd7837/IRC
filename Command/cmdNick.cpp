#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdNick(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = _clients[clientFd];
    std::string oldNick = cli->getNick();

    if (!cli->getCert())
        clientCert(clientFd);
    if (!cli->getCert())
        return ;
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        std::string nickname = it->second->getNick();
        if (nickname == parsedMessage.getParams()[0])
        {
            codeMsgReply(clientFd, 407);
            return ;
        }
    }
    cli->setNick(parsedMessage.getParams()[0]);

    if (oldNick != "")
        bcastMsg(":" + oldNick + " NICK " + parsedMessage.getParams()[0] + "\r\n");
}