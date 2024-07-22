#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

void Server::cmdNick(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    if (cli == nullptr)
        return ;
    
    std::string oldNick = cli->getNick();

    for (std::map<int, Client*>::iterator it = _clients.begin(); it != _clients.end(); it++)
    {
        std::string nickname = it->second->getNick();
        if (nickname == parsedMessage.getParams()[0])
        {
            codeMsgReply(clientFd, 407);
            return ;
        }
    }
    cli->setNick(parsedMessage.getParams()[0]);

    if (oldNick != "")
        bcastMsg(":" + oldNick + " NICK " + parsedMessage.getParams()[0] + "\r\n");
}