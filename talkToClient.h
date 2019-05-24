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
    std::string GetLogin();
    void SetLogin(std::string);
    void Stop();

private:
    ip::tcp::socket _socket;
    char _buff[MAX_BUFF_SIZE];
    std::string _login;
    size_t _alreadyRead;

    size_t readComplete(const boost::system::error_code &, size_t);
};

#endif // TALKTOCLIENT_H
