#include "chatServer.h"

int main()
{
    auto server = new ChatServer();
    server->Run();

    return 0;
}
