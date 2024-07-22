#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"

void Server::bcastMsg(std::string msg)
{
    msg += "\r\n";
    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        if (send(it->first, msg.c_str(), msg.size(), 0) < 0)
            throw(std::runtime_error("send() failed"));
    }
}