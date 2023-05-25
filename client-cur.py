import socket

SERVER_IP = '127.0.0.1'
PORT = 8080
MAX_BUFFER_SIZE = 1024


def main():
    client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client_socket.connect((SERVER_IP, PORT))

    print("서버에 연결되었습니다.")

    while True:
        expression = input("수식 입력: ")

        if not expression:
            print("연결을 종료합니다.")
            break

        client_socket.send(expression.encode())

        result = client_socket.recv(MAX_BUFFER_SIZE).decode()
        print("계산 결과:", result)

    client_socket.close()


if __name__ == '__main__':
    main()
