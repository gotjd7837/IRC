#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

static unsigned int modeFlag(char c)
{
    switch(c)
    {
        case 'O': return (MODE_O);
        case 'P': return (MODE_P);
        case 'S': return (MODE_S);
        case 'I': return (MODE_I);
        case 'T': return (MODE_T);
        case 'N': return (MODE_N);
        case 'M': return (MODE_M);
        case 'L': return (MODE_L);
        case 'B': return (MODE_B);
        case 'V': return (MODE_V);
        case 'K': return (MODE_K);
        default: return (0);  // 정의되지 않은 문자의 경우 0 반환
    }
}

void Server::cmdMode(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    Channel* cha = getChannel(parsedMessage.getParams()[0]);

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
}   