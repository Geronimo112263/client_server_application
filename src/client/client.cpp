#include "../../headers/client/client.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

Client::Client() {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);

  if (fd_ == -1) {
    perror("socket error");
    exit(EXIT_FAILURE);
  }

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(8080);
}

void Client::run() {
  if (inet_pton(addr_.sin_family, "127.0.0.1", &addr_.sin_addr) <= 0) {
    std::cerr << "invalid address\n";
    exit(EXIT_FAILURE);
  }

  if (connect(fd_, (const sockaddr *)&addr_, sizeof(addr_)) == -1) {
    perror("cnnect error\n");
    exit(EXIT_FAILURE);
  }

  writeMessage();

  close(fd_);
}

void Client::writeMessage() {
  std::string message;
  std::cout << "Введите сообщение: ";
  std::getline(std::cin, message);

  send(fd_, message.c_str(), message.size(), 0);

  char buffer[512] = {0};
  int bytes = read(fd_, buffer, sizeof(buffer));

  if (bytes > 0) {
    std::cout << "Сервер: " << std::string(buffer, bytes) << std::endl;
  }
}