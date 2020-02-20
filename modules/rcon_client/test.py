#!/usb/bin/python3

import socket
UDP_IP = "1.1.1.1"
UDP_PORT = 5555


byte_array = bytearray([255, 255, 255, 255])
byte_array.extend(b"challenge rcon")

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.connect((UDP_IP, UDP_PORT))
sock.send(byte_array)

data, addr = sock.recvfrom(4096)
#print ("received message:", data)

#print (data[:4]) # must be 4 x \xFF

master_key=data[4:-2].decode("utf-8").split(' ')[2];

#print(master_key)

byte_status = bytearray([255, 255, 255, 255])
byte_status.extend(b"rcon ")
byte_status.extend(master_key.encode())
byte_status.extend(b" mypass status")

#print(byte_status);

sock.send(byte_status)

status, _ = sock.recvfrom(4096) 
print (status[5:-2].decode("utf-8"))

sock.close()
