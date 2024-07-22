#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdUser(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = _clients[clientFd];

    if (!cli->getCert())
        clientCert(clientFd);
    if (!cli->getCert())
        return ;
    if (cli->getNick() == "")
    {
        codeMsgReply(clientFd, 461);
        return ;
    }

    cli->setUser(parsedMessage.getParams()[0]);

    std::string welcomeMessage = "001 " + cli->getNick() + " :Welcome to the Internet Relay Network " + cli->getNick() + "\r\n";
    bcastMsg(welcomeMessage);
}