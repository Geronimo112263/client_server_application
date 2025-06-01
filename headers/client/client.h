#ifndef _CLIENT_H_
#define _CLIENT_H_

#include <netinet/in.h>
#include <sys/socket.h>

class Client {
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

#endif