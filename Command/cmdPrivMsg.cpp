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

    if (!cli->isRegistered())
    {
        ucastMsg(clientFd, "451 PRIVMSG :You have not registered");
        return ;
    }
    if (parsedMessage.getParams().size() < 2)
        ucastMsg(clientFd, std::string("461 " + cli->getNick() + " PRIVMSG :Not enough parameters"));

    std::vector<std::string>targetlist = split(parsedMessage.getParams()[0], ',');

    for (std::vector<std::string>::iterator it = targetlist.begin(); it != targetlist.end(); it++)
    {
        if (isalpha((*it)[0]) || isdigit((*it)[0])) // to user
        {
            std::map<int, Client*>::iterator cliit;
            for (cliit = _clients.begin(); cliit != _clients.end(); cliit++)
            {
                if (cliit->second->getNick() == (*it))
                {
                    ucastMsg(cliit->first, ":" + cli->getNick() + " PRIVMSG " + (*it) + " :" + parsedMessage.getParams()[1]);
                    break ;
                }
            }
            if (cliit == _clients.end())
                ucastMsg(clientFd, std::string("401 " + cli->getNick() + " " + (*it) + " :No such nick/channel"));
        }
        else // to channel
        {
            std::map<std::string, Channel*>::iterator chait = _channels.find(*it);
            if (chait == _channels.end())
                ucastMsg(clientFd, std::string("403 " + cli->getNick() + " " + (*it) + " :No such channel"));
            else
            {
                if (!chait->second->isMember(cli))
                {
                    ucastMsg(clientFd, std::string("442 " + cli->getNick() + " " + chait->first + " :You're not on that channel"));
                    return ;
                }
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