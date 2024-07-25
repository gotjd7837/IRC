#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdPass(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    if (cli == nullptr)
        return ;

    if (parsedMessage.getParams().empty())
        ucastMsg(clientFd, "461 PASS : Not enough parameters");
    if (cli->getCert())
        ucastMsg(clientFd, "462 PASS :You are already registered");

    cli->setPass(parsedMessage.getParams()[0]);
}