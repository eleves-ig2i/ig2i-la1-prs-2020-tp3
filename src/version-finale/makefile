CC=gcc
EXE_DIR=./
W_FLAGS=-Wall
V_FINAL_DIR=./version-finale/

all: vfinale

vfinale: bal_serveur_final bal_client_final

bal_serveur_final: clients.o ipc_utils.o utils.o bal_serveur_final.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal_client_final: clients.o ipc_utils.o utils.o bal_client_final.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
clients.o: clients.c
	gcc -c clients.c

utils.o: utils.c
	gcc -c utils.c
	
ipc_utils.o: ipc_utils.c
	gcc -c ipc_utils.c
	
clean:
	rm -f *.exe *.o
