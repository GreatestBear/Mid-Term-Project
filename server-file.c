#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];

    // 소켓 생성
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    memset(serverAddr.sin_zero, '\0', sizeof(serverAddr.sin_zero));

    // 소켓과 서버 주소 바인딩
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // 클라이언트로부터 연결 요청 대기
    if (listen(serverSocket, 1) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", PORT);

    // 클라이언트 연결 수락
    if ((clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addrLen)) == -1) {
        perror("Accepting failed");
        exit(EXIT_FAILURE);
    }

    // 클라이언트로부터 파일명 수신
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(clientSocket, buffer, BUFFER_SIZE, 0) == -1) {
        perror("Receiving failed");
        exit(EXIT_FAILURE);
    }

    // 파일 열기
    FILE *file = fopen(buffer, "r");
    if (file == NULL) {
        perror("File opening failed");
        exit(EXIT_FAILURE);
    }

    // 파일 내용 읽기
    memset(buffer, 0, BUFFER_SIZE);
    while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
        // 클라이언트로 파일 내용 전송
        if (send(clientSocket, buffer, strlen(buffer), 0) == -1) {
            perror("Sending failed");
            exit(EXIT_FAILURE);
        }
        memset(buffer, 0, BUFFER_SIZE);
    }

    // 파일 닫기
    fclose(file);

    // 클라이언트 소켓 종료
    close(clientSocket);

    // 서버 소켓 종료
    close(serverSocket);

    return 0;
}
