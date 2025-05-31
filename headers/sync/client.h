#include <sys/socket.h>
#include <netinet/in.h>

class Client{
public:
    Client();

    Client(int) = delete;
    Client(Client&&) = delete;
    Client(const Client&) = delete;

    Client& operator=(const Client&) = delete;
    Client& operator=(Client&&) = delete;

    void run();
private:
    int fd_;
    struct sockaddr_in addr_;

    void writeMessage();
};