#include "talkToClient.h"

TalkToClient::TalkToClient(io_service* service)
    : _socket{ip::tcp::socket(*service)}
{
}

ip::tcp::socket & TalkToClient::Socket() {
    return _socket;
}

void TalkToClient::Stop() {
    boost::system::error_code err;
    _socket.close(err);
}

std::string TalkToClient::ReadRequest() {
    if (_socket.available()) {
        _alreadyRead = 0;
        read(_socket,
             buffer(_buff),
             std::bind(&TalkToClient::readComplete, this, std::placeholders::_1, std::placeholders::_2));
       return std::string(_buff, _alreadyRead - 1);
    }

    return "";
}

size_t TalkToClient::readComplete(const boost::system::error_code &err, size_t bytes) {
    if (err) return 0;

    _alreadyRead = bytes;
    bool found = std::find(_buff, _buff + bytes, '\n') < _buff + bytes;
    return found ? 0 : 1;
}

void TalkToClient::SendRequest(std::string msg) {
    _socket.write_some(buffer(msg + "\n"));
}

std::string TalkToClient::GetLogin() {
    return _login;
}

void TalkToClient::SetLogin(std::string login) {
    _login = login;
}
