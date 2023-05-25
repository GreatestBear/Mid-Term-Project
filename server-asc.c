#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080
#define MAX_BUFFER_SIZE 1024

int main() {
    int serverSocket, newSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[MAX_BUFFER_SIZE];

    // 소켓 생성
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        perror("소켓 생성 실패");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    // 소켓 바인딩
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("바인딩 실패");
        exit(EXIT_FAILURE);
    }

    // 클라이언트의 연결 요청 대기
    if (listen(serverSocket, 5) == -1) {
        perror("리스닝 실패");
        exit(EXIT_FAILURE);
    }

    printf("서버가 연결 요청을 기다리는 중...\n");

    // 클라이언트 연결 수락
    socklen_t clientAddrLen = sizeof(clientAddr);
    newSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (newSocket == -1) {
        perror("연결 수락 실패");
        exit(EXIT_FAILURE);
    }

    printf("클라이언트가 연결되었습니다.\n");

    // 클라이언트로부터 데이터 수신 및 변환 후 전송
    while (1) {
        memset(buffer, 0, sizeof(buffer));

        // 클라이언트로부터 데이터 수신
        if (recv(newSocket, buffer, sizeof(buffer), 0) == -1) {
            perror("수신 실패");
            exit(EXIT_FAILURE);
        }

        if (strlen(buffer) == 0) {
            printf("클라이언트가 연결을 종료했습니다.\n");
            break;
        }

        printf("수신한 문자열: %s\n", buffer);

        // 아스키 코드로 변환
        char asciiBuffer[MAX_BUFFER_SIZE * 3];  // 변환된 아스키 코드 문자열을 저장할 버퍼
        memset(asciiBuffer, 0, sizeof(asciiBuffer));

        for (int i = 0; i < strlen(buffer); i++) {
            sprintf(asciiBuffer + strlen(asciiBuffer), "%d ", (int)buffer[i]);
        }

        printf("변환된 아스키 코드: %s\n", asciiBuffer);

        // 변환된 아스키 코드 전송
        if (send(newSocket, asciiBuffer, strlen(asciiBuffer), 0) == -1) {
            perror("전송 실패");
            exit(EXIT_FAILURE);
        }
    }

    // 소켓 닫기
    close(newSocket);
    close(serverSocket);

    return 0;
}
