#include <sys/socket.h>
#include <netinet/in.h>

#include <vector>
#include <thread>
#include <mutex>

class Server{
public:
    Server();
    explicit Server(int port);

    void connectingTheClient();
    
private:
    int serverFd_;
    sockaddr_in addr_{};

    std::vector<std::thread> threads_;
    std::mutex mutexFd_;

    void bindSocket();
    void listenSocket();
    void handleClient(int clientFd);
    
};