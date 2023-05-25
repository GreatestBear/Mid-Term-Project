import socket
import math

SERVER_IP = '127.0.0.1'
PORT = 8080
MAX_BUFFER_SIZE = 1024


def calculate_expression(expression):
    try:
        result = eval(expression)
        return str(result)
    except Exception as e:
        print("수식 계산 중 오류 발생:", e)
        return "Error"


def main():
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server_socket.bind((SERVER_IP, PORT))
    server_socket.listen(1)

    print("서버가 연결 요청을 기다리는 중...")

    client_socket, client_addr = server_socket.accept()
    print("클라이언트가 연결되었습니다.")

    while True:
        data = client_socket.recv(MAX_BUFFER_SIZE).decode()

        if not data:
            print("클라이언트가 연결을 종료했습니다.")
            break

        print("수식 입력:", data)
        result = calculate_expression(data)
        print("계산 결과:", result)

        client_socket.send(result.encode())

    client_socket.close()
    server_socket.close()


if __name__ == '__main__':
    main()
