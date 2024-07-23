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
        codeMsgReply(clientFd, 461);
        removeClient(clientFd);
        return ;
    }
    else
    {
        codeMsgReply(clientFd, 464);
        removeClient(clientFd);
        return ;
    }
}