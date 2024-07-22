#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::ucastMsg(int clientFd, std::string msg)
{
    msg += "\r\n";

    if (send(clientFd, msg.c_str(), msg.size(), 0) < 0)
        throw(std::runtime_error("send() failed"));
}