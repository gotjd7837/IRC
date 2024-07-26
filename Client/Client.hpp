#ifndef CLIENT_HPP

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>

#define RED "\e[1;31m" //-> for red color
#define WHI "\e[0;37m" //-> for white color
#define GRE "\e[1;32m" //-> for green color
#define YEL "\e[1;33m" //-> for yellow color

class Client
{
    private :
        int _fd;
        bool _cert;
        std::string _ipaddr;
        std::string _nickname;
        std::string _username;
        std::string _realname;
        std::string _password;
        std::string _messageBuff;
    
    public :
        Client();
        int getFd();
        bool getCert();
        std::string getPass();
        std::string getNick();
        std::string getUser();

        void setUser(std::string user);
        void setNick(std::string nick);
        void setPass(std::string pass);
        void setCert(bool cert);
        void setFd(int fd);
        void setIpaddr(std::string ipaddr);

        std::string popMessageBuff();
        void pushMessageBuff(std::string& message);

        std::string getPrefix();
};

#endif
