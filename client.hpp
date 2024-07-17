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
        std::string _ipadd;

        std::string _messageBuff;
    
    public :
        Client();
        int getfd();

        void setfd(int fd);
        void setipadd(std::string ipadd);

        std::string popMessageBuff();
        void pushMessageBuff(std::string& message);

};

#endif
