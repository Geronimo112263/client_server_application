#ifndef _SYNC_SERVER_H_
#define _SYNC_SERVER_H_

#include <netinet/in.h>
#include <sys/socket.h>

#include <mutex>
#include <thread>
#include <vector>

class Server {
public:
  Server();
  explicit Server(int port);
  ~Server();

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

#endif