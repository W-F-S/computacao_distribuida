# Nome dos executáveis
CLIENT = client
SERVER = server

# Fontes
RPCGEN = rpcgen
RPCFILE = simp.x
CLIENT_SRC = simp_clnt.c simp_xdr.c simpclient.c
SERVER_SRC = simp_svc.c simp_xdr.c simpservice.c

# Flags
CC = gcc
CFLAGS = -Wall
LIBS = -ltirpc

# Targets padrão
all: $(CLIENT) $(SERVER)

# Gerar arquivos RPC automaticamente
simp_clnt.c simp_svc.c simp_xdr.c simp.h: $(RPCFILE)
	$(RPCGEN) -C $(RPCFILE)

# Compilar cliente
$(CLIENT): $(CLIENT_SRC) simp.h
	$(CC) $(CFLAGS) -o $(CLIENT) $(CLIENT_SRC) $(LIBS)

# Compilar servidor
$(SERVER): $(SERVER_SRC) simp.h
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC) $(LIBS)

# Limpeza de arquivos gerados
clean:
	rm -f *.o $(CLIENT) $(SERVER) simp_clnt.c simp_svc.c simp_xdr.c simp.h

