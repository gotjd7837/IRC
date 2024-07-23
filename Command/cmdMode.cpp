#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

static unsigned int modeFlag(char c)
{
    switch(c)
    {
        case 'o': return (MODE_O);
        case 'p': return (MODE_P);
        case 's': return (MODE_S);
        case 'i': return (MODE_I);
        case 't': return (MODE_T);
        case 'n': return (MODE_N);
        case 'm': return (MODE_M);
        case 'l': return (MODE_L);
        case 'b': return (MODE_B);
        case 'v': return (MODE_V);
        case 'k': return (MODE_K);
        default: return (0);  // 정의되지 않은 문자의 경우 0 반환
    }
}

void Server::cmdMode(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    Channel* cha = getChannel(parsedMessage.getParams()[0]);

    if (cha == NULL)
        return ;
    unsigned int add = 0, remove = 0;

    std::string tmp = parsedMessage.getParams()[1];
    for (int i = 1; i < tmp.size(); i++)
    {
        if (tmp[0] == '+')
            add |= modeFlag(tmp[i]);
        else if (tmp[0] == '-')
            remove |= modeFlag(tmp[i]);
    }

    cha->addMode(add);
    cha->removeMode(remove);

    ccastMsg(cha->getName(), std::string("324 " + cli->getNick() + " " + cha->getName() + " " + cha->getMode()));
}   