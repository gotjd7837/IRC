#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdNick(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = _clients[clientFd];

    if (!cli->_cert)
        clientCert(clientFd);
    if (!cli->_cert)
        return ;
    for (std::map<int, Client *>::iterator it = _client.begin(); it != _client.end(); it++)
    {
        
    }
    
    
}