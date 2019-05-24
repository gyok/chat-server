#ifndef CHATSERVER_H
#define CHATSERVER_H

#define PORT 2001

#include <boost/thread.hpp>
#include <boost/chrono.hpp>
#include <boost/asio.hpp>
#include "talkToClient.h"

using namespace boost::asio;

class ChatServer {
private:
    typedef std::shared_ptr<TalkToClient> clientPtr;
public:
    typedef std::vector<clientPtr> array;
    static array clients;
    static io_service service;
    static std::recursive_mutex mutex;

    void Run();
private:
    static constexpr const char* _password = "12345";
    void static acceptThread();
    void static clientsThread();
    bool static loginClient(clientPtr, std::string &);
    void static notifyAboutNewUser(array &, std::string);
    void static sendToAllClients(array &, std::string);
    bool static handleMsg(std::string, array &, clientPtr);
};


#endif // CHATSERVER_H
