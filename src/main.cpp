#include <pthread.h>
#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "Request.h"
#include "Response.h"

class Client {
public:
    Client(struct sockaddr_in* cli_addr, socklen_t clilen, int sock)
            : cli_addr(cli_addr), clilen(clilen), sock(sock){}

    struct sockaddr_in* cli_addr;
    socklen_t clilen;
    int sock;
};

void *clientProcess(void* args) {
    Client* cli = (Client*)args;
    char rcv_msg[4096];
    Response response;
    time_t begTime = clock();
    while (true) {
        long byte_num = recv(cli->sock, rcv_msg, 4096, 0);
        if (byte_num < 0) {
            std::cout << "connection broken" << std::endl;
            break;
        }
        rcv_msg[byte_num] = '\0';

        if (byte_num > 0) {
            std::cout << rcv_msg << std::endl;
        }

        Request request(rcv_msg, byte_num);
        if (request.METHOD == "GET") {
            std::string res;
            response.responseGET(request.PATH, res);
            send(cli->sock, res.c_str(), res.length(), 0);
            std::cout << "***************" << std::endl;
            std::cout << res << std::endl;
        }

    }
    close(cli->sock);
    delete cli;
    std::cout << "close successfully" << std::endl;
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, const char *argv[]) {

    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        std::cout << "create listen socket failed" << std::endl;
        return 1;
    }

    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    int portno = 2489;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cout << "bind failed" << std::endl;
        return 1;
    }

    std::cout << "Server established successfully." << std::endl;

    listen(sockfd, 5);

    std::cout << "Listening ..." << std::endl;

    while (1) {
        struct sockaddr_in* cli_addr = (struct sockaddr_in*)malloc(sizeof(struct sockaddr_in));
        bzero((char*)cli_addr, sizeof(struct sockaddr_in));
        socklen_t clilen = sizeof(cli_addr);
        int newsockfd = accept(sockfd, (struct sockaddr*)cli_addr, &clilen);
        if (newsockfd < 0) {
            std::cout << "create client socket failed" << std::endl;
            continue;
        }
        Client *newcli = new Client(cli_addr, clilen, newsockfd);
        pthread_t pid;
        pthread_create(&pid, NULL, clientProcess, (void*)newcli);
        std::cout << "A new client connected" << std::endl;
    }

    return 0;
}