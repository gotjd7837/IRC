#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

/*
** cmdKick
** 1. param이 2개 미만일 경우 461 파라미터 부족 에러를 전송한다.
** 2. 채널이 존재하지 않을 경우 403 채널이 존재하지 않음 에러를 전송한다.
** 3. 클라이언트가 채널에 속해있지 않을 경우 442 채널에 속해있지 않음 에러를 전송한다.
** 4. 클라이언트가 operator가 아닐 경우 482 operator 권한이 없음 에러를 전송한다.
** 5. 채널에서 클라이언트를 제거한다.
** 6. 채널에 속해있는 클라이언트에게 채널에서 나갔다는 메시지를 전송한다.
** 6-1. kick의 메세지가 있을 경우 해당 메세지를 전송한다.
** 6-2. kick의 메세지가 없을 경우 채널에서 나갔다는 메세지를 전송한다.
** 7. 타겟 클라이언트가 없을 경우 401 클라이언트가 존재하지 않음 에러를 전송한다.


*/

void Server::cmdKick(MessageProtocol& parsedMessage, int clientFd)
{
    if (parsedMessage.getParams().size() < 2)
    {
        ucastMsg(clientFd, std::string("461 " + getClient(clientFd)->getNick() + " KICK :Not enough parameters"));
        return ;
    }

    std::string channelName = parsedMessage.getParams()[0];
    Client* client = getClient(clientFd);
    Channel* channel = getChannel(channelName);
    if (channel == nullptr)
    {
        ucastMsg(clientFd, std::string("403 " + client->getNick() + " " + channelName + " :No such channel"));
        return ;
    }
    if (channel->isOper(client) == false)
    {
        ucastMsg(clientFd, std::string("482 " + client->getNick() + " :You're not channel operator"));
        return ;
    }

    std::string targetNick = parsedMessage.getParams()[1];
    Client* targetClient = channel->getMember(targetNick);
    if (targetClient == nullptr)
    {
        ucastMsg(clientFd, std::string("441 " + client->getNick() + " " + targetNick + " :They aren't on that channel"));
        return ;
    }

    ccastMsg(channelName, std::string(client->getPrefix() + " KICK " + channelName + " " + targetNick + " " + parsedMessage.getParams()[2]));
    channel->removeMember(targetClient);
}