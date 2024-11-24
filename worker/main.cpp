#include <algorithm>
#include <cmath>
#include <cstring>
#include <iostream>

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <netinet/in.h>

double f(double x) {
    return cos(x) + sin(x);
}

const size_t N = 1e6;

double integrate(double l, double r) {
    double len = (r - l) / (double)N;
    double answer = 0;
    double cur = l;

    for (size_t i = 0; i < N; i++) { 
        double end = cur + len;
        double middle = (cur + end) / 2;
        answer += f(middle) * len;
        cur += len;
    }

    return answer;
}

const char PING[] = "HELLO";
const char ACK[] = "ACK";

const int UDP_PORT = 12101;
const int TCP_PORT = 11179;

int main() {
    int taskFd;
    if ((taskFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket failed");
        return -1;
    }

    int opt = 1;
    if (setsockopt(taskFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt");
        return -1;
    }

    struct sockaddr_in taskAddress;

    taskAddress.sin_family = AF_INET;
    taskAddress.sin_addr.s_addr = INADDR_ANY; 
    taskAddress.sin_port = htons(TCP_PORT); 

    if (bind(taskFd, (struct sockaddr *)&taskAddress, sizeof(taskAddress)) < 0) {
        perror("bind failed");
        return -1;
    }

    if (listen(taskFd, 6) < 0) {
        perror("listen failed");
        return -1;
    }

    int pingFd;
    if ((pingFd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("sock error\n");
        return -1;
    }

    struct sockaddr_in serverAddr;

    memset((void*)&serverAddr, 0, sizeof(struct sockaddr_in));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = htons(INADDR_ANY);
    serverAddr.sin_port = htons(UDP_PORT);

    if (bind(pingFd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("bind error");
        return -1;
    }

    char buffer[1024];

    struct sockaddr_in clientAddr;

    while (true) {
        int n = std::max(pingFd, taskFd) + 1;
        fd_set readfd;
        FD_ZERO(&readfd);
        FD_SET(pingFd, &readfd);
        FD_SET(taskFd, &readfd);

        int amount = select(n, &readfd, NULL, NULL, 0);
        if (amount <= 0) {
            continue;
        }
        if (FD_ISSET(pingFd, &readfd)) {
            socklen_t fromlen = sizeof(struct sockaddr_in);
            int len = recvfrom(pingFd, buffer, sizeof(buffer), 0, (struct sockaddr*)&clientAddr, &fromlen);
            if (std::strcmp(buffer, PING) == 0) {
                std::cout << "GOT PING FROM IP " << inet_ntoa(clientAddr.sin_addr) << " PORT " << ntohs(clientAddr.sin_port) << std::endl;
                std::memcpy(buffer, ACK, sizeof(ACK));
                len = sendto(pingFd, buffer, strlen(buffer), 0, (struct sockaddr*)&clientAddr, fromlen);
            }
        }
    }
}
