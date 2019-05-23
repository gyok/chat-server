#include "talkToClient.h"

TalkToClient::TalkToClient(io_service* service)
    : _socket{ip::tcp::socket(*service)}
{
}

ip::tcp::socket & TalkToClient::Socket() {
    return _socket;
}

void TalkToClient::stop() {
    boost::system::error_code err;
    _socket.close(err);
}

std::string TalkToClient::ReadRequest() {
    if (_socket.available()) {
       _socket.read_some(buffer(_buff));
       return std::string(_buff);
    }

    return "";
}

void TalkToClient::SendRequest(std::string msg) {
    _socket.write_some(buffer(msg + "\n"));
}
