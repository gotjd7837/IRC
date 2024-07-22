#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

static std::vector<std::string> split(const std::string& str, char delimiter) 
{
    std::vector<std::string> tokens;
    size_t start = 0;
    size_t end = str.find(delimiter);

    while (end != std::string::npos) 
    {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
        end = str.find(delimiter, start);
    }
    tokens.push_back(str.substr(start));

    return tokens;
}

void Server::cmdJoin(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);

    if (parsedMessage.getParams().empty())
        codeMsgReply(clientFd, 461);
    
    std::vector<std::string>targetChannels = split(parsedMessage.getParams()[0], ',');
    std::vector<std::string>targetKeys;
    if (parsedMessage.getParams().size() > 1)
        std::vector<std::string>targetKeys = split(parsedMessage.getParams()[1], ',');

    for (int i = 0; i < targetChannels.size(); i++)
    {
        std::string targetChannel = targetChannels[i];
        std::string targetKey = "";
        bool op = false;

        if (i < targetKeys.size())
            targetKey = targetKeys[i];

        if (_channels.find(targetChannel) == _channels.end())
        {
            op = true;
            _channels[targetChannel] = new Channel(targetChannel, targetKey);
        }

        Channel* channel = _channels.find(targetChannel)->second;
        if (targetKey == channel->getKey())
        {
            channel->addMember(cli, op);
            ucastMsg(clientFd, std::string("332 " + targetChannel + " :" + "Welcome to the Foobar channel!"));

            std::string users = "";
            for(std::map<Client *, bool>::const_iterator it = channel->getMembers().begin(); it != channel->getMembers().end(); it++)
                users += it->first->getNick() + " ";
            ucastMsg(clientFd, std::string("353 " + targetChannel + " :" + users));

            ucastMsg(clientFd, std::string("363 " + targetChannel + " :" + "End of /NAMES list"));
        }
        else
            codeMsgReply(clientFd, 475);
    }
}
