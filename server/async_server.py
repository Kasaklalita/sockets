import logging
import socket
import time
from multiprocessing import Process

from decouple import config

address = config('ADDRESS')
port = config('PORT', cast=int)
delay = config('DELAY', cast=int)
author = config('AUTHOR')

logging.basicConfig(
  handlers=[logging.FileHandler('server.log'), logging.StreamHandler()],
  format='%(asctime)s [%(levelname)s] %(message)s',
  level=logging.INFO
)

def handle_client(conn, addr):
  logging.info(f'Connected with {addr}')
  data = conn.recv(1024)
  logging.info(f'{addr} said:\'{data.decode()}\'1')
  message = data.decode()[::-1] + f' Server was created by {author}'
  time.sleep(delay)
  conn.send(message.encode())
  logging.info(f'Server responded \'{message}\' to {addr}')
  conn.close()

try:
  s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  s.bind((address, port))
  s.listen(5)
  s.settimeout(999)
  logging.info('Server has been launched')

  while True:
    conn, addr = s.accept()
    p = Process(target=handle_client, args=(conn, addr))
    p.start()
except:
  pass
finally:
  logging.info(f'Server has been shut down')
  s.close()