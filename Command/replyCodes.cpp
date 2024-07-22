#include "../Server/Server.hpp"
#include "../Client/Client.hpp"
#include "../MessageProtocol/MessageProtocol.hpp"
#include "../Channel/Channel.hpp"

#define ERR_NOSUCHNICK ":No such nick/channel"
#define ERR_NOSUCHSERVER ":No such server"
#define ERR_NOSUCHCHANNEL ":No such channel"
#define ERR_CANNOTSENDTOCHAN ":Cannot send to channel"
#define ERR_TOOMANYCHANNELS ":Too many channels"
#define ERR_WASNOSUCHNICK ":There was no such nick"
#define ERR_NICKNAMEINUSE ":Nickname is already in use"
#define ERR_USERNOTINCHANNEL ":User not in channel"
#define ERR_NOTONCHANNEL ":Not on channel"
#define ERR_USERONCHANNEL ":User is already on channel"
#define ERR_NOLOGIN ":User not logged in"
#define ERR_SUMMONDISABLED ":SUMMON has been disabled"
#define ERR_USERSDISABLED ":USERS has been disabled"
#define ERR_NOTREGISTERED ":You have not registered"
#define ERR_NEEDMOREPARAMS ":Not enough parameters"
#define ERR_ALREADYREGISTRED ":You are already registered"
#define ERR_NOPERMFORHOST ":No permission for host"
#define ERR_PASSWDMISMATCH ":Password incorrect"
#define ERR_YOUREBANNEDCREEP ":You are banned from this server"
#define ERR_KEYSET ":Channel key already set"
#define ERR_CHANNELISFULL ":Channel is full"
#define ERR_UNKNOWNMODE ":Unknown mode"
#define ERR_INVITEONLYCHAN ":Invite only channel"
#define ERR_BANNEDFROMCHAN ":You are banned from channel"
#define ERR_BADCHANNELKEY ":Bad channel key"
#define ERR_BADCHANMASK ":Invalid channel mask"
#define ERR_NOCHANMODES ":Channel modes are not supported"
#define ERR_BANLISTFULL ":Ban list is full"
#define ERR_NOPRIVILEGES ":Permission Denied"
#define ERR_CHANOPRIVSNEEDED ":You're not channel operator"
#define ERR_CANTKILLSERVER ":You can't kill a server"
#define ERR_NOOPERHOST ":No host specified"
#define ERR_UMODEUNKNOWNFLAG ":Unknown mode flag"
#define ERR_USERSDONTMATCH ":Can't change mode for other users"

void Server::sendMsg(int clientFd, char *code, std::string nick, char *msg)
{
    ucastMsg(clientFd, ":"+ _name + " " + std::string(code) + " " + nick + " " + std::string(msg));
}

void Server::codeMsgReply(int clientFd, int code)
{
    std::string nick = _clients[clientFd]->getNick();

    if (code == 401) sendMsg(clientFd, "401", nick, ERR_NOSUCHNICK);
    if (code == 402) sendMsg(clientFd, "402", nick, ERR_NOSUCHSERVER);
    if (code == 403) sendMsg(clientFd, "403", nick, ERR_NOSUCHCHANNEL);
    if (code == 404) sendMsg(clientFd, "404", nick, ERR_CANNOTSENDTOCHAN);
    if (code == 405) sendMsg(clientFd, "405", nick, ERR_TOOMANYCHANNELS);
    if (code == 406) sendMsg(clientFd, "406", nick, ERR_WASNOSUCHNICK);
    if (code == 407) sendMsg(clientFd, "407", nick, ERR_NICKNAMEINUSE);
    if (code == 441) sendMsg(clientFd, "441", nick, ERR_USERNOTINCHANNEL);
    if (code == 442) sendMsg(clientFd, "442", nick, ERR_NOTONCHANNEL);
    if (code == 443) sendMsg(clientFd, "443", nick, ERR_USERONCHANNEL);
    if (code == 444) sendMsg(clientFd, "444", nick, ERR_NOLOGIN);
    if (code == 445) sendMsg(clientFd, "445", nick, ERR_SUMMONDISABLED);
    if (code == 446) sendMsg(clientFd, "446", nick, ERR_USERSDISABLED);
    if (code == 451) sendMsg(clientFd, "451", nick, ERR_NOTREGISTERED);
    if (code == 461) sendMsg(clientFd, "461", nick, ERR_NEEDMOREPARAMS);
    if (code == 462) sendMsg(clientFd, "462", nick, ERR_ALREADYREGISTRED);
    if (code == 463) sendMsg(clientFd, "463", nick, ERR_NOPERMFORHOST);
    if (code == 464) sendMsg(clientFd, "464", nick, ERR_PASSWDMISMATCH);
    if (code == 465) sendMsg(clientFd, "465", nick, ERR_YOUREBANNEDCREEP);
    if (code == 467) sendMsg(clientFd, "467", nick, ERR_KEYSET);
    if (code == 471) sendMsg(clientFd, "471", nick, ERR_CHANNELISFULL);
    if (code == 472) sendMsg(clientFd, "472", nick, ERR_UNKNOWNMODE);
    if (code == 473) sendMsg(clientFd, "473", nick, ERR_INVITEONLYCHAN);
    if (code == 474) sendMsg(clientFd, "474", nick, ERR_BANNEDFROMCHAN);
    if (code == 475) sendMsg(clientFd, "475", nick, ERR_BADCHANNELKEY);
    if (code == 476) sendMsg(clientFd, "476", nick, ERR_BADCHANMASK);
    if (code == 477) sendMsg(clientFd, "477", nick, ERR_NOCHANMODES);
    if (code == 478) sendMsg(clientFd, "478", nick, ERR_BANLISTFULL);
    if (code == 481) sendMsg(clientFd, "481", nick, ERR_NOPRIVILEGES);
    if (code == 482) sendMsg(clientFd, "482", nick, ERR_CHANOPRIVSNEEDED);
    if (code == 483) sendMsg(clientFd, "483", nick, ERR_CANTKILLSERVER);
    if (code == 491) sendMsg(clientFd, "491", nick, ERR_NOOPERHOST);
    if (code == 501) sendMsg(clientFd, "501", nick, ERR_UMODEUNKNOWNFLAG);
    if (code == 502) sendMsg(clientFd, "502", nick, ERR_USERSDONTMATCH);
}