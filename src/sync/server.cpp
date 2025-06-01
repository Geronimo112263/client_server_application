#include <unistd.h>

#include <cstdio>
#include <iostream>

#include "../../headers/sync/sync_server.h"

Server::Server() {
  serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFd_ == -1) {
    perror("error occurs for socket");
    exit(EXIT_FAILURE);
  }

  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = INADDR_ANY;
  addr_.sin_port = htons(8080);

  bindSocket();
  listenSocket();
}

Server::Server(int port) {
  serverFd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (serverFd_ == -1) {
    perror("error occurs for socket");
    exit(EXIT_FAILURE);
  }

  addr_.sin_family = AF_INET;
  addr_.sin_addr.s_addr = INADDR_ANY;
  addr_.sin_port = htons(port);

  bindSocket();
  listenSocket();
}

Server::~Server() {
  for (auto &t : threads_) {
    t.join();
  }
}

void Server::bindSocket() {
  if (bind(serverFd_, (const sockaddr *)&addr_, sizeof(addr_)) == -1) {
    perror("error bind");
    exit(EXIT_FAILURE);
  }
}

void Server::listenSocket() { listen(serverFd_, 8); }

void Server::connectingTheClient() {
  while (true) {
    int clientFd = accept(serverFd_, nullptr, nullptr);

    if (clientFd >= 0) {
      std::lock_guard<std::mutex> lock(mutexFd_);
      std::cout << "[+] Подключен новый клиент: " << clientFd << std::endl;

      threads_.emplace_back(
          [this, clientFd]() { this->handleClient(clientFd); });
    }
  }
}

void Server::handleClient(int clientFd) {
  char buffer[512] = {0};
  int bytes = read(clientFd, buffer, sizeof(buffer));

  if (bytes > 0) {
    std::cout << "Клиент " << clientFd << ": " << buffer << std::endl;
    std::string returnBack = "Сервер: PONG\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    if (send(clientFd, returnBack.c_str(), sizeof(returnBack), 0) == -1) {
      perror("fail send");
    }
    close(clientFd);

    std::cout << "[-] Клиент " << clientFd << " ушел" << std::endl;
  }
}
