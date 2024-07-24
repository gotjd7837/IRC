#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

static unsigned int modeFlag(char c)
{
    switch(c)
    {
        case 'i': return (MODE_I); //가능 , 초대 전용 모드 채널으로 할건지 말지
        case 't': return (MODE_T); //가능 , 토픽 변경 권한 오퍼레이터만 가능하게 할건지 아닌지


        case 'o': return (MODE_O); // 불가능 , 채널의 유저에게 오퍼권한 부여할건지 말건지
        case 'k': return (MODE_K); // 불가능 , 채널에 비밀번호 설정할건지 말건지
        case 'l': return (MODE_L); //불가능 , 채널에 유저 제한 걸건지 말건지
        //case 'p': return (MODE_P);
        //case 's': return (MODE_S);
        //case 'n': return (MODE_N);
        //case 'm': return (MODE_M);
        //case 'b': return (MODE_B);
        //case 'v': return (MODE_V);
        default: return (0);
    }
}

void Server::cmdMode(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);
    Channel* cha = getChannel(parsedMessage.getParams()[0]);

    if (cha == NULL)
        return ;
    if (parsedMessage.getParams().size() < 2)
        return ;
    unsigned int add = 0, remove = 0;

    std::string tmp = parsedMessage.getParams()[1];
    if (tmp == "+o")
    {
        return ;
    }
    if (tmp == "-o")
    {
        return ;
    }
    if (tmp == "+k")
    {
        return ;
    }
    if (tmp == "-k")
    {
        return ;
    }
    if (tmp == "+l")
    {
        return ;
    }
    if (tmp == "-l")
    {
        return ;
    }

    for (int i = 1; i < tmp.size(); i++)
    {
        unsigned int bit = 0;
        if (tmp[0] == '+')
        {
            bit = modeFlag(tmp[i]);
            if (bit == 0) 
            {
                ucastMsg(clientFd, std::string("472 " + getClient(clientFd)->getNick() 
                + " " + parsedMessage.getParams()[1] + " :is unknown mode char to me"));
                return ;
            }
            add |= bit;
        }
        else if (tmp[0] == '-')
        {
            bit = modeFlag(tmp[i]);
            if (bit == 0) 
            {
                ucastMsg(clientFd, std::string("472 " + getClient(clientFd)->getNick() 
                + " " + parsedMessage.getParams()[1] + " :is unknown mode char to me"));
                return ;
            }
            remove |= bit;
        }
        else
        {
            ucastMsg(clientFd, std::string("472 " + getClient(clientFd)->getNick() 
            + " " + parsedMessage.getParams()[1] + " :is unknown mode char to me"));
            return ;
        }
    }
    cha->addMode(add);
    cha->removeMode(remove);

    // ccastMsg(cha->getName(), std::string("324 " + cli->getNick() + " " + cha->getName() + " " + cha->getMode()));
    ccastMsg(cha->getName(), std::string(cli->getPrefix() + " MODE " + cha->getName() + " " + parsedMessage.getParams()[1]));
}   