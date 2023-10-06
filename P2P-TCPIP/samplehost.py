import socket

# Create a socket object
server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

# Define the server's IP address and port
server_ip = '0.0.0.0'  # Listen on all available interfaces
server_port = 12345

# Bind the socket to the IP address and port
server_socket.bind((server_ip, server_port))

# Listen for incoming connections
server_socket.listen(5)

print(f"Server listening on {server_ip}:{server_port}")

# Accept incoming connections
client_socket, client_address = server_socket.accept()
print(f"Connection from {client_address}")

# Receive data from the client
data = client_socket.recv(1024)
print(f"Received: {data.decode('utf-8')}")

# Send a response back to the client
response = "Hello from Windows!"
client_socket.send(response.encode('utf-8'))

# Close the connection
client_socket.close()
server_socket.close()
