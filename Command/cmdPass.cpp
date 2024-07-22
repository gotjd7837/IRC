#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdPass(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = _clients[clientFd];

    if (parsedMessage.getParams().empty())
        codeMsgReply(clientFd, 461);
    if (cli->_cert)
        codeMsgReply(clientFd, 462);

    cli->_password = parsedMessage.getParams()[0];
}