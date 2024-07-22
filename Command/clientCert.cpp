#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::clientCert(int clientFd)
{
    Client* cli = _clients[clientFd];

    if (cli->getPass() == this->_password)
        cli->setCert(true);
    else if (cli->getPass() == "")
    {
        codeMsgReply(clientFd, 461);
        removeClient(clientFd);
    }
    else
    {
        codeMsgReply(clientFd, 464);
        removeClient(clientFd);
    }
}