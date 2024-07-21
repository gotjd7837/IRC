#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::cmdPass(MessageProtocol& parsedMessage, int clientFd)
{
    std::string pass = parsedMessage.getParams()[0]; 
}