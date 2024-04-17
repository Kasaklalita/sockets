import socket

from decouple import config

address = config('ADDRESS')
port = config('PORT', cast=int)
delay = config('DELAY', cast=int)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect((address, port))
sock.send(bytes('Hello, world', encoding='UTF-8'))
data = sock.recv(1024)
sock.close()
print(data)