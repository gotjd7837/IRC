#include "../Server/Server.hpp"

void Server::ucastMsg(int clientFd, std::string msg)
{
    msg += "\r\n";

    std::cout << GRE << "send to <" << clientFd << "> : " << msg << WHI << std::endl;
    send(clientFd, msg.c_str(), msg.size(), 0);
}