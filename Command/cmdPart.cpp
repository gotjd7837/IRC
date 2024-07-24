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
** 7. 채널에 아무도 없을 경우 채널을 삭제한다.
*/

void Server::cmdPart(MessageProtocol& parsedMessage, int clientFd)
{
    std::string channelName = parsedMessage.getParams()[0];
    Client* client = getClient(clientFd);
    Channel* channel = getChannel(channelName);

    if (parsedMessage.getParams().empty())
    {
        ucastMsg(clientFd, std::string("461 " + client->getNick() + " PART :Not enough parameters"));
        return ;
    }
    if (channel == nullptr)
    {
        ucastMsg(clientFd, std::string("403 " + client->getNick() + " " + channelName + " :No such channel"));
        return ;
    }
    if (channel->isMember(client) == false)
    {
        ucastMsg(clientFd, std::string("442 " + client->getNick() + " " + channelName + " :You're not on that channel"));
        return ;
    }
    
    ccastMsg(channelName, std::string(client->getPrefix() + " PART " + channelName + " " + parsedMessage.getParams()[1]));
    ucastMsg(clientFd, std::string("442 " + client->getNick() + " " + channelName + " :You have left the channel"));
    channel->removeMember(client);
    if (channel->getMembers().empty())
        removeChannel(channelName);
}
