#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <stdexcept>

#define SERVER_IP "127.0.0.1"  // 서버 IP
#define SERVER_PORT 6667        // 서버 포트

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    // 소켓 생성
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cerr << "Socket creation error" << std::endl;
        return 1;
    }

    // 서버 주소 초기화
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // IP 주소 변환
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        return 1;
    }

    // 서버에 연결
    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        std::cerr << "Connection Failed" << std::endl;
        return 1;
    }

    std::cout << "Connected to server" << std::endl;

    // 데이터 전송 및 수신
    while (true) {
        std::cout << "Enter message to send (or 'quit' to exit): ";
        std::string message;
        std::getline(std::cin, message);

        if (message == "quit") {
            break;
        }

        // 메시지 전송
        send(sockfd, message.c_str(), message.length(), 0);

        // 서버로부터 응답 수신
        // ssize_t bytes = recv(sockfd, buffer, sizeof(buffer), 0);
        // if (bytes == -1) {
        //     std::cerr << "Recv error" << std::endl;
        //     break;
        // } else if (bytes == 0) {
        //     std::cerr << "Server disconnected" << std::endl;
        //     break;
        // } else {
        //     buffer[bytes] = '\0';
        //     std::cout << "Server response: " << buffer << std::endl;
        // }
    }

    // 연결 종료
    close(sockfd);
    std::cout << "Disconnected from server" << std::endl;

    return 0;
}