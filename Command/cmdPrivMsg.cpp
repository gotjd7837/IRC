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

void Server::cmdPrivMsg(MessageProtocol& parsedMessage, int clientFd)
{
    Client* cli = getClient(clientFd);

    if (parsedMessage.getParams().size() < 2)
        codeMsgReply(clientFd, 461);

    std::vector<std::string>targetlist = split(parsedMessage.getParams()[0], ',');

    for (std::vector<std::string>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
    {
        if (isalpha((*it)[0])) // to user
        {
            for (std::map<int, Client*>::iterator cliit = _clients.begin(); cliit != _clients.end(); cliit++)
            {
                if (cliit->second->getNick() == (*it))
                {
                    ucastMsg(cliit->first, ":" + cli->getNick() + " PRIVMSG " + (*it) + " :" + parsedMessage.getParams()[1]);
                    break ;
                }
            }
        }
        else // to channel
        {
            for (std::map<std::string, Channel*>::iterator chait = _channels.begin(); chait != _channels.end(); chait++)
            {
                if (chait->first == (*it))
                {
                    std::map<Client*, bool>members = chait->second->getMembers();
                    for (std::map<Client*, bool>::iterator memit = members.begin(); memit != members.end(); memit++)
                    {
                        if (memit->first->getNick() == cli->getNick())
                            continue ;
                        ucastMsg(memit->first->getFd(), ":" + cli->getNick() + " PRIVMSG " + (*it) + " " + parsedMessage.getParams()[1]);
                    }
                    break ;
                }
            }
        }
    }
}