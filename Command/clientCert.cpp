#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::clientCert(int clientFd)
{
    Client* cli = _clients[clientFd];

    if (cli->_password == this->_password)
        cli->_cert = true;
    else if (cli->_password == "")
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