#include <fcntl.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include "../../headers/async/async_server.h"

Server::Server() {
  serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFd_ == -1) {
    perror("error occurs for socket");
    exit(EXIT_FAILURE);
  }

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(8080);
  addr_.sin_addr.s_addr = INADDR_ANY;
}

Server::Server(const int &port) {
  serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFd_ == -1) {
    perror("error occurs for socket");
    exit(EXIT_FAILURE);
  }

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
  addr_.sin_addr.s_addr = INADDR_ANY;
}

Server::~Server() {
  {
    std::lock_guard<std::mutex> lock(mutexServ_);
    for (auto &t : threads_) {
      if (t.joinable()) {
        t.join();
      }
    }
  }
  close(serverFd_);
}

int Server::setNonBlocking(const int &fd) {
  int flag = fcntl(fd, F_GETFL, 0);
  return fcntl(fd, F_SETFL, flag | O_NONBLOCK);
}

void Server::run() {
  int bindSocket = bind(serverFd_, (sockaddr *)&addr_, sizeof(addr_));

  if (bindSocket == -1) {
    perror("fail bind");
    exit(EXIT_FAILURE);
  }

  int listenSocket = listen(serverFd_, 8);

  if (listenSocket == -1) {
    perror("fail listen");
    exit(EXIT_FAILURE);
  }

  if (setNonBlocking(serverFd_) == -1) {
    perror("fcntl error");
    close(serverFd_);
    exit(EXIT_FAILURE);
  }

  fds_.push_back({serverFd_, POLLIN, 0});
  connectingTheClient();
}

void Server::connectingTheClient() {
  while (true) {
    int count = poll(fds_.data(), fds_.size(), 1000);

    if (count < 0) {
      perror("fail poll");
      break;
    }

    for (size_t i = 0; i < fds_.size(); ++i) {
      if (fds_[i].revents & POLLIN) {
        if (fds_[i].fd == serverFd_) {
          int clientFd = accept(serverFd_, nullptr, nullptr);

          if (clientFd >= 0) {
            if (setNonBlocking(clientFd) == -1) {
              perror("fcntl error");
              close(clientFd);
              continue;
            }

            fds_.push_back({clientFd, POLLIN, 0});
            std::cout << "[+] Новый клиент: " << clientFd << std::endl;
          }

        } else {
          int clientFd = fds_[i].fd;
          {
            std::lock_guard<std::mutex> lock(mutexServ_);
            threads_.emplace_back(
                [this, clientFd]() { handleClient(clientFd); });
          }
          fds_.erase(fds_.begin() + i);
          --i;
        }
      }
    }
  }
}

void Server::handleClient(const int &clientFd) {
  int flags = fcntl(clientFd, F_GETFL, 0);
  fcntl(clientFd, F_SETFL, flags & ~O_NONBLOCK);

  char buffer[64] = {0};
  int count = 0;

  while (count < 3) {
    memset(buffer, 0, sizeof(buffer));
    int bytes = read(clientFd, buffer, sizeof(buffer));

    if (bytes > 0) {
      std::string received(buffer, bytes);
      std::cout << "Клиент " << clientFd << ": " << received << std::endl;

      if (received == "PING") {
        std::string returnBack = "PONG";
        std::this_thread::sleep_for(std::chrono::milliseconds(3000));
        send(clientFd, returnBack.c_str(), returnBack.size(), 0);
        ++count;
      }
    } else if (bytes == 0) {
      std::cout << "Клиент " << clientFd << " отключился\n";
      break;
    } else {
      perror("fail read");
      break;
    }
  }

  close(clientFd);
  std::cout << "[-] Клиент " << clientFd << " отключен\n";
}
