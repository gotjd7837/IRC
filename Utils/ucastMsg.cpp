#include "../Server/Server.hpp"

void Server::ucastMsg(int clientFd, std::string msg)
{
    msg += "\r\n";

    send(clientFd, msg.c_str(), msg.size(), 0);
}