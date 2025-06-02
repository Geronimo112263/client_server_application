#ifndef _ASYNC_SERVER_H_
#define _ASYNC_SERVER_H_

#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <thread>
#include <vector>

class Server {
public:
  Server();
  explicit Server(const int &);
  ~Server();

  void run();

private:
  int serverFd_;
  sockaddr_in addr_{};

  std::vector<std::thread> threads_;
  std::mutex mutexServ_;

  std::vector<pollfd> fds_;

  int setNonBlocking(const int &);
  void connectingTheClient();
  void handleClient(const int &);
};

#endif