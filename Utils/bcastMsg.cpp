#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::bcastMsg(std::string msg)
{
    msg += "\r\n";

    std::cout << GRE << "send to all : " << msg << WHI << std::endl;
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
        send(it->first, msg.c_str(), msg.size(), 0);
}