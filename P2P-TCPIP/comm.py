import socket

# Define the server's IP address and port to connect to

server_ip = '192.168.5.178'  # Replace with the actual server's IP address
server_port = 12345  # Replace with the actual server's port number

# Create a socket object
client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
    # Connect to the server
    client_socket.connect((server_ip, server_port))
except Exception as e:
    print("Error:", str(e))

    
# Receive data from the server
data = client_socket.recv(1024)

# Check if the server has closed the connection
if not data:
    print("Connection closed by the server.")
else:
    # Decode and print the received data
    print("Received:", data.decode('utf-8'))
