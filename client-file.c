#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[BUFFER_SIZE];

    // 소켓 생성
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("Invalid address/ Address not supported");
        exit(EXIT_FAILURE);
    }
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    // 파일명 입력 받기
    printf("Enter the file name: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strcspn(buffer, "\n")] = '\0'; // 개행 문자 제거

    // 서버로 파일명 전송
    if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
        perror("Sending failed");
        exit(EXIT_FAILURE);
    }

    // 서버로부터 파일 내용 수신
    memset(buffer, 0, BUFFER_SIZE);
    while (recv(clientSocket, buffer, BUFFER_SIZE, 0) > 0) {
        printf("%s", buffer);
        memset(buffer, 0, BUFFER_SIZE);
    }

    // 클라이언트 소켓 종료
    close(clientSocket);

    return 0;
}
