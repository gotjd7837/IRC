#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <vector>
# include <map>

# define MODE_O 0x1
# define MODE_P 0x2
# define MODE_S 0x4
# define MODE_I 0x8
# define MODE_T 0x10
# define MODE_N 0x20
# define MODE_M 0x40
# define MODE_L 0x80
# define MODE_B 0x100
# define MODE_V 0x200
# define MODE_K 0x400

class Client;

class Channel
{
private:
    std::string _name;
    std::string _topic;
    std::string _key;
    unsigned int _limit;
    unsigned int _modes;
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

    unsigned int getLimit() const;
    void setLimit(unsigned int limit);

    // Getter and Setter for _members
    const std::map<Client*, bool>& getMembers() const;
    void setMembers(const std::map<Client*, bool>& members);
    Client* getMember(const std::string& nick) const;
    void addOper(Client* client);
    void removeOper(Client* client);

    // Additional methods for managing members
    void addMember(Client* client, bool status);
    void removeMember(Client* client);
    bool isMember(Client* client) const;
    bool isOper(Client* client) const;

    bool checkLimit() const;
    bool checkKey(const std::string& key) const;

    void addMode(unsigned int mode);
    void removeMode(unsigned int mode);
    bool hasMode(unsigned int mode) const;
    std::string getMode() const;
};

#endif