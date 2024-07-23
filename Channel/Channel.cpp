#include "Channel.hpp"
#include "../Client/Client.hpp"

Channel::Channel(std::string name, std::string key) : _name(name), _key(key), _modes(0x0) {}
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

Client* Channel::searchMemberNick(const std::string& nick) const 
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
bool Channel::hasMode(unsigned int mode)
{
    return (_modes & mode);
}