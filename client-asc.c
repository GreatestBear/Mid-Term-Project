#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int clientSocket;
    struct sockaddr_in serverAddr;
    char buffer[MAX_BUFFER_SIZE];

    // 소켓 생성
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);

    // 서버 IP 주소 설정
    if (inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr)) <= 0) {
        perror("IP 주소 설정 실패");
        exit(EXIT_FAILURE);
    }

    // 서버에 연결
    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("연결 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버에 연결되었습니다.\n");

    // 사용자 입력 받고 서버로 전송
    while (1) {
        printf("문자열 입력: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';  // 개행 문자 제거

        if (strlen(buffer) == 0) {
            printf("연결을 종료합니다.\n");
            break;
        }

        // 서버로 데이터 전송
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            perror("전송 실패");
            exit(EXIT_FAILURE);
        }

        memset(buffer, 0, sizeof(buffer));

        // 서버로부터 데이터 수신
        if (recv(clientSocket, buffer, sizeof(buffer), 0) == -1) {
            perror("수신 실패");
            exit(EXIT_FAILURE);
        }

        printf("변환된 아스키 코드: %s\n", buffer);
    }

    // 소켓 닫기
    close(clientSocket);

    return 0;
}
