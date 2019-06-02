#include "chatServer.h"

ChatServer::array ChatServer::clients;
io_service ChatServer::service;
std::recursive_mutex ChatServer::mutex;
std::string ChatServer::secret_word;

void ChatServer::Run() {
    boost::thread_group threads;
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
            if (!handleMsg(msg, clients, (*b))) break;
            if (msg != "") {
                if (msg == ChatServer::secret_word) {
                    sendToAllClients(clients, "[=>" + (*b)->GetLogin() + "]: won with word " + msg);
                } else {
                    sendToAllClients(clients, "[" + (*b)->GetLogin() + "]: " + msg);
                }
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
    ChatServer::sendToAllClients(clients, login + " joined game");
}

void ChatServer::sendToAllClients(array & clients, std::string msg) {
    for (array::iterator b = clients.begin(), e = clients.end(); b != e; b++) {
        (*b)->SendRequest(msg);
    }
}

bool ChatServer::handleMsg(std::string msg, array & clients, clientPtr client) {
    if (client->GetLogin() != "Admin") return true;

    auto delimiter = ' ';
    auto spacePos = msg.find(delimiter);
    auto firstWord = msg.substr(0, spacePos);

    if (firstWord != "@kick") {
        if (firstWord == "") return true;
        //return true;
        secret_word = firstWord;
        return false;
    }

    auto userToKick = msg.substr(spacePos + 1, msg.length());
    auto filteredClients = std::remove_if(clients.begin(), clients.end(), [&userToKick](clientPtr client) {
        if (client->GetLogin() != userToKick) return false;
        client->Stop();
        return true;
    });
    clients.erase(filteredClients, clients.end());
    return false;
}
