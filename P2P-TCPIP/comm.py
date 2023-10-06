import socket

SERVER_HOST = '192.168.7.240'  # Replace with the actual IP address of the Windows server
SERVER_PORT = 12345

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((SERVER_HOST, SERVER_PORT))

while True:
    message = input("Enter a message to send to Windows (or 'exit' to quit): ")
    if message.lower() == 'exit':
        break
    client_socket.send(message.encode())

    response = client_socket.recv(1024)
    print(f"Received from Windows: {response.decode()}")

client_socket.close()