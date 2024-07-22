#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdPong(MessageProtocol& parsedMessage, int clientFd)
{
    ucastMsg(clientFd, std::string("PONG " + parsedMessage.getParams()[0]));
}