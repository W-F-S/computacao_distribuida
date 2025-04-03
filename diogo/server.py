import socket
import threading
import os

HOST = '0.0.0.0'
PORT = 5000

def handle_client(conn, addr):
    print(f"[NOVA CONEXÃO] Cliente {addr} conectado.")

    try:
        filename = conn.recv(1024).decode()
        if not filename:
            conn.sendall(b"ERROR_UNKNOWN")
            return

        if os.path.exists(filename):
            conn.sendall(b"OK")
            with open(filename, "rb") as f:
                while chunk := f.read(1024):
                    conn.sendall(chunk)
        else:
            conn.sendall(b"ERROR_FILE_NOT_FOUND")
    except Exception as e:
        print(f"[ERRO] {e}")
        conn.sendall(b"ERROR_UNKNOWN")
    finally:
        conn.close()
        print(f"[CONEXÃO ENCERRADA] Cliente {addr} desconectado.")

def start_server():
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind((HOST, PORT))
    server.listen(5)
    print(f"[SERVIDOR] Rodando em {HOST}:{PORT}")

    while True:
        conn, addr = server.accept()
        client_thread = threading.Thread(target=handle_client, args=(conn, addr))
        client_thread.start()

if __name__ == "__main__":
    start_server()
