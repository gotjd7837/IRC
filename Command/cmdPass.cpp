#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdPass(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    if (cli == nullptr)
        return ;

    if (parsedMessage.getParams().empty())
        codeMsgReply(clientFd, 461);
    if (cli->getCert())
        codeMsgReply(clientFd, 462);

    cli->setPass(parsedMessage.getParams()[0]);
}