import socket

SERVER_IP = '127.0.0.1'
PORT = 5000

def request_file(filename):
    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    client.connect((SERVER_IP, PORT))
    client.sendall(filename.encode())

    response = client.recv(1024).decode()

    if response == "OK":
        with open(f"download_{filename}", "wb") as f:
            while chunk := client.recv(1024):
                f.write(chunk)
        print(f"Arquivo '{filename}' baixado com sucesso!")
    elif response == "ERROR_FILE_NOT_FOUND":
        print("Erro: Arquivo não encontrado no servidor.")
    else:
        print("Erro desconhecido ao solicitar arquivo.")

    client.close()

if __name__ == "__main__":
    filename = input("Digite o nome do arquivo: ")
    request_file(filename)
