#include "Channel.hpp"
#include "../Client/Client.hpp"

Channel::Channel(std::string name, std::string key) : _name(name), _key(key), _limit(UINT_MAX), _modes(0x0) {}

// Getter and Setter for _name
const std::string& Channel::getName() const 
{
    return _name;
}

void Channel::setKey(const std::string& key) 
{
    _key = key;
}

const std::string& Channel::getKey() const 
{
    return _key;
}

void Channel::setName(const std::string& name) 
{
    _name = name;
}

// Getter and Setter for _topic
const std::string& Channel::getTopic() const 
{
    return _topic;
}

void Channel::setTopic(const std::string& topic) 
{
    _topic = topic;
}

unsigned int Channel::getLimit() const 
{
    return _limit;
}

void Channel::setLimit(unsigned int limit) 
{
    _limit = limit;
}

bool Channel::checkLimit() const 
{
    return (_members.size() < _limit);
}

// Getter and Setter for _members
const std::map<Client*, bool>& Channel::getMembers() const 
{
    return _members;
}

void Channel::setMembers(const std::map<Client*, bool>& members) 
{
    _members = members;
}

// Additional methods for managing members
void Channel::addMember(Client* client, bool status) 
{
    _members[client] = status;
}

void Channel::removeMember(Client* client) 
{
    _members.erase(client);
}

bool Channel::isMember(Client* client) const 
{
    return (_members.find(client) != _members.end());
}

bool Channel::isOper(Client* client) const 
{
    return (_members.find(client) != _members.end() && _members.find(client)->second);
}

void Channel::addOper(Client* client) 
{
    _members.find(client)->second = true;
}

void Channel::removeOper(Client* client) 
{
    _members.find(client)->second = false;
}

Client* Channel::getMember(const std::string& nick) const 
{
    for (std::map<Client*, bool>::const_iterator it = _members.begin(); it != _members.end(); it++)
    {
        if (it->first->getNick() == nick)
            return (it->first);
    }
    return (nullptr);
}

void Channel::addMode(unsigned int mode)
{
    _modes |= mode;
}
void Channel::removeMode(unsigned int mode)
{
    _modes &= ~mode;
}
bool Channel::hasMode(unsigned int mode) const
{
    return (_modes & mode);
}

std::string Channel::getMode() const 
{
    std::string mode = "+";

    if (hasMode(MODE_O)) mode += "o";
    if (hasMode(MODE_P)) mode += "p";
    if (hasMode(MODE_S)) mode += "s";
    if (hasMode(MODE_I)) mode += "i";
    if (hasMode(MODE_T)) mode += "t";
    if (hasMode(MODE_N)) mode += "n";
    if (hasMode(MODE_M)) mode += "m";
    if (hasMode(MODE_L)) mode += "l";
    if (hasMode(MODE_B)) mode += "b";
    if (hasMode(MODE_V)) mode += "v";
    if (hasMode(MODE_K)) mode += "k";

    return mode;
}

void Channel::addInvite(std::string clientName)
{
    _invited.insert(clientName);
}

void Channel::removeInvite(std::string clientName)
{
    _invited.erase(clientName);
}

bool Channel::isInvited(std::string clientName)
{
    return (_invited.find(clientName) != _invited.end());
}