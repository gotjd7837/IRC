#include "../Server/Server.hpp"
#include "../Client/Client.hpp"

void Server::ucastMsg(int clientFd, std::string msg)
{
    Client* cli = getClient(clientFd);
    std::string sendMsg = cli->popSendBuff() + msg;

    std::string data = sendMsg + "\r\n";
    if (send(clientFd, data.c_str(), data.size(), 0) == -1)
    {
        cli->pushSendBuff(sendMsg);
        return ;
    }
    std::cout << GRE << "send to <" << clientFd << "> : " << sendMsg << WHI << std::endl;
}