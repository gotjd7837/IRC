#include "MessageProtocol.hpp"

MessageProtocol::MessageProtocol() {}

MessageProtocol::MessageProtocol(std::string& message) : _buff(message)
{
    makePrefix();
    makeCommand();
    makeParams();
}

MessageProtocol::~MessageProtocol() {}

MessageProtocol::MessageProtocol(const MessageProtocol& other)
{
    _buff = other._buff;
    _prefix = other._prefix;
    _command = other._command;
    _params = other._params;
}

MessageProtocol& MessageProtocol::operator=(const MessageProtocol& other)
{
    _buff = other._buff;
    _prefix = other._prefix;
    _command = other._command;
    _params = other._params;
    return (*this);
}

void MessageProtocol::makePrefix()
{
    int i = 0;

    if (_buff[0] != ':')
        _prefix = "";
    else
    {
        while (i != _buff.size())
        {
            if (_buff[i] != ' ')
                _prefix += _buff[i];
            i++;
            if (_buff[i] != ' ' && _buff[i - 1] == ' ')
                break ;
        }
        _buff = _buff.substr(i, _buff.length() - i);
    }
}

void MessageProtocol::makeCommand()
{
    int i = 0;

    while (i != _buff.size())
    {
        if (_buff[i] == ' ')
            break ;
        _command += _buff[i];
        i++;
    }
    _buff = _buff.substr(i, _buff.length() - i);
}

void MessageProtocol::makeParams()
{
    std::string tmp = "";

    if (_buff.size() == 0 || _buff[0] != ' ')
        throw (std::runtime_error("클라이언트 메세지 형식 err"));
    
    for (int i = 0; i < _buff.size(); i++)
    {
        if (_buff[i] == ' ')
            continue ;
        else if (_buff[i] == ':')
        {
            tmp = _buff.substr(i);
            _params.push_back(tmp);
            return ;
        }
        else
        {
            while (i < _buff.size() && _buff[i] != ' ')
                tmp += _buff[i++];
            _params.push_back(tmp);
            tmp.clear();
        }
    }
}

std::string MessageProtocol::getPrefix()
{
    return (_prefix);
}

std::string MessageProtocol::getCommand()
{
    return (_command);
}

std::vector<std::string> MessageProtocol::getParams()
{
    return (_params);
}
