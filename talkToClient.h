#ifndef TALKTOCLIENT_H
#define TALKTOCLIENT_H


#define MAX_BUFF_SIZE 1024

#include <memory>
#include <boost/asio.hpp>
#include <string>

using namespace boost::asio;

struct TalkToClient
    : std::enable_shared_from_this<TalkToClient> {
public:
    TalkToClient(io_service*);
    ip::tcp::socket & Socket();
    std::string ReadRequest();
    void SendRequest(std::string);

private:
    ip::tcp::socket _socket;
    char _buff[MAX_BUFF_SIZE];

    void stop();
};

#endif // TALKTOCLIENT_H
