#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::clientCert(int clientFd)
{
    Client* cli = getClient(clientFd);
    if (cli == nullptr)
        return ;

    if (cli->getPass() == this->_password)
        cli->setCert(true);
    else if (cli->getPass() == "")
    {
        ucastMsg(clientFd, "461 " + cli->getNick() + " :need to PASS first");
        removeClient(clientFd);
        return ;
    }
    else
    {
        ucastMsg(clientFd, "464 " + cli->getNick() + " :incorrect password");
        removeClient(clientFd);
        return ;
    }
}