#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

/**/

void Server::cmdOper(MessageProtocol& parsedMessage, int clientFd)
{
    // Client* cli = getClient(clientFd);

    // if (parsedMessage.getParams().size() < 2)
    // {
    //     codeMsgReply(clientFd, 461);
    //     return ;
    // }

    // std::string operName = parsedMessage.getParams()[0];
    // std::string operPass = parsedMessage.getParams()[1];
    // if (operName != "oper" || operPass != _password)
    // {
    //     codeMsgReply(clientFd, 464);
    //     return ;
    // }

    // cli->setOper(true);
    // codeMsgReply(clientFd, 381);
}