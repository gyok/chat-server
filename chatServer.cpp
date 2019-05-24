#include "chatServer.h"

ChatServer::array ChatServer::clients;
io_service ChatServer::service;
std::recursive_mutex ChatServer::mutex;

void ChatServer::Run() {
    boost::thread_group threads;
    //acceptThread();
    threads.create_thread(acceptThread);
    threads.create_thread(clientsThread);
    threads.join_all();
}

void ChatServer::acceptThread() {
    ip::tcp::acceptor acceptor(service, ip::tcp::endpoint(ip::tcp::v4(), PORT));
    while (true) {
        clientPtr client(new TalkToClient(&service));
        acceptor.accept(client->Socket());
        std::string login;
        if (loginClient(client, login)) {
            mutex.lock();
            client->SetLogin(login);
            clients.push_back(client);
            notifyAboutNewUser(clients, login);
            mutex.unlock();
        } else {
            client->Stop();
        }
    }
}

void ChatServer::clientsThread() {
    std::string msg;
    while (true) {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(100));
        mutex.lock();
        for (array::iterator b = clients.begin(), e = clients.end(); b != e; b++) {
            msg = (*b)->ReadRequest();
            if (msg != "") {
                sendToAllClients(clients, "[" + (*b)->GetLogin() + "]: " + msg);
            }
        }
        mutex.unlock();
    }
}

bool ChatServer::loginClient(clientPtr client,
                             std::string & login) {
    std::string msg = client->ReadRequest();
    auto delimiter = ' ';
    auto spacePos = msg.find(delimiter);
    login = msg.substr(0, spacePos);
    std::string password = msg.substr(spacePos + 1, msg.length());

    return password == _password;
}

void ChatServer::notifyAboutNewUser(array & clients, std::string login) {
    ChatServer::sendToAllClients(clients, login + " joined chat");
}

void ChatServer::sendToAllClients(array & clients, std::string msg) {
    for (array::iterator b = clients.begin(), e = clients.end(); b != e; b++) {
        (*b)->SendRequest(msg);
    }
}
