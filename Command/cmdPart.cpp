#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

/*
** cmdPart
** 1. param이 없을 경우 461 파라미터 부족 에러를 전송한다.
** 2. 채널이 존재하지 않을 경우 403 채널이 존재하지 않음 에러를 전송한다.
** 3. 클라이언트가 채널에 속해있지 않을 경우 442 채널에 속해있지 않음 에러를 전송한다.
** 4. 채널에서 클라이언트를 제거한다.
** 5. 331 채널에서 나감 에러를 전송한다.

** 6. 채널에 속해있는 클라이언트에게 채널에서 나갔다는 메시지를 전송한다.
** 6-1. part의 메세지가 있을 경우 해당 메세지를 전송한다.
** 6-2. part의 메세지가 없을 경우 채널에서 나갔다는 메세지를 전송한다.
*/

void Server::cmdPart(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);

    if (parsedMessage.getParams().empty())
        codeMsgReply(clientFd, 461);

    std::string channelName = parsedMessage.getParams()[0];
    if (_channels.find(channelName) == _channels.end())
        codeMsgReply(clientFd, 403);
    if (_channels[channelName]->isMember(cli) == false)
        codeMsgReply(clientFd, 442);

    _channels[channelName]->removeMember(cli);
    if (parsedMessage.getParams().size() > 1)
    {
        ucastMsg(clientFd, std::string(":" + cli->getNick() + " PART " + channelName + " " + parsedMessage.getParams()[1]));
        for (std::map<Client*, bool>::const_iterator it = _channels[channelName]->getMembers().begin(); it != _channels[channelName]->getMembers().end(); it++)
            ucastMsg(it->first->getFd(), std::string(":" + cli->getNick() + " PART " + channelName + " " + parsedMessage.getParams()[1]));
    }
    else
    {
        ucastMsg(clientFd, std::string(":" + cli->getNick() + " PART " + channelName));
        for (std::map<Client*, bool>::const_iterator it = _channels[channelName]->getMembers().begin(); it != _channels[channelName]->getMembers().end(); it++)
            ucastMsg(it->first->getFd(), std::string(":" + cli->getNick() + " PART " + channelName));
    }

}
