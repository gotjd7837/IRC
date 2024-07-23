#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdMode(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);

    
}