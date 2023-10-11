import socket

HOST = '0.0.0.0'  # Listen on all available network interfaces
PORT = 12345

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen()

print(f"Server (Windows) is listening on {HOST}:{PORT}")

client_socket, client_address = server_socket.accept()
print(f"Connected to {client_address}")

while True:
    response = input("Enter a response: ")
    client_socket.send(response.encode())
    data = client_socket.recv(1024)
    if not data:
        break
    print(f"Received from Windows: {data.decode()}")


client_socket.close()
server_socket.close()
