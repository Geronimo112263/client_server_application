#include "../../headers/client/client.h"

#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <iostream>

#include <chrono>
#include <ctime>
#include <iomanip>

#include <thread>

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

Client::~Client() {
  auto now = std::chrono::system_clock::now();
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                now.time_since_epoch()) %
            1000;
  auto now_time_t = std::chrono::system_clock::to_time_t(now);

  std::tm *tm = std::localtime(&now_time_t);

  std::cout << "Завершение работы клиента: " << std::put_time(tm, "%H:%M:%S")
            << "." << std::setfill('0') << std::setw(3) << ms.count()
            << std::endl;
}

void Client::writeMessage() {
  std::string message = "PING";
  char buffer[64] = {0};
  int count = 0;

  while (count != 3) {
    memset(buffer, 0, sizeof(buffer));

    if (send(fd_, message.c_str(), message.size(), 0) == -1) {
      perror("fail send");
      break;
    }

    int bytes = read(fd_, buffer, sizeof(buffer));

    if (bytes > 0) {
      std::cout << std::string(buffer, bytes) << std::endl;
      count++;
    } else if (bytes == -1) {
      perror("fail read");
      break;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  }
}