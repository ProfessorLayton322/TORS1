#include <cstring>

#include <stdlib.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <linux/in.h>
#include <sys/socket.h>
#include <sys/select.h>

const char message[] = "HELLO WORLD\n";

int main(int argc, char* argv[]) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0) {
        perror("sock error");
        return -1;
    }

    int soBroadcast = 1;
    if (setsockopt(
        sock,
        SOL_SOCKET,
        SO_BROADCAST,
        &soBroadcast,
        sizeof soBroadcast)) {
        perror("setsockopt error");
    }

    struct sockaddr_in broadcastAddr;
    memset((void*)&broadcastAddr, 0, sizeof(broadcastAddr));
    broadcastAddr.sin_family = AF_INET;
    broadcastAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
    broadcastAddr.sin_port = htons(12101);

    char* leftEnv = getenv("LEFT");
    double left;
    sscanf(leftEnv, "%lf", &left);

    char* rightEnv = getenv("RIGHT");
    double right;
    sscanf(rightEnv, "%lf", &right);

    sendto(sock, message, sizeof(message), 0, (struct sockaddr*) &broadcastAddr, sizeof(broadcastAddr));

    while (true) {
    }
}
