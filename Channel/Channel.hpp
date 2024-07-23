#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>

class Client;

class Channel
{
private:
    std::string _name;
    std::string _topic;
    std::string _modes;
    std::string _key;
    std::map<Client*, bool> _members;

public:

    Channel(std::string name, std::string key = "");
    // Getter and Setter for _name
    const std::string& getName() const;
    void setName(const std::string& name);

    const std::string& getKey() const;
    void setKey(const std::string& key);

    // Getter and Setter for _topic
    const std::string& getTopic() const;
    void setTopic(const std::string& topic);

    // Getter and Setter for _modes
    const std::string& getModes() const;
    void setModes(const std::string& modes);

    // Getter and Setter for _members
    const std::map<Client*, bool>& getMembers() const;
    void setMembers(const std::map<Client*, bool>& members);

    // Additional methods for managing members
    void addMember(Client* client, bool status);
    void removeMember(Client* client);
    bool isMember(Client* client) const;
    bool isOper(Client* client) const;
    Client* searchMemberNick(const std::string& nick) const;
};

#endif