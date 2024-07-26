#ifndef MESSAGEPROTOCOL_HPP

#include <iostream>
#include <string>
#include <vector>


class MessageProtocol
{
    private:
        std::string _buff;
        std::string _prefix;
        std::string _command;
        std::vector<std::string> _params;

    public:
        MessageProtocol();
        MessageProtocol(std::string& message);
        ~MessageProtocol();
        MessageProtocol(const MessageProtocol& other);
        MessageProtocol& operator=(const MessageProtocol& other);

        void makePrefix();
        void makeCommand();
        void makeParams();

        std::string getPrefix();
        std::string getCommand();
        std::vector<std::string> getParams();
};


#endif