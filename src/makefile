CC=gcc
EXE_DIR=../out/
W_FLAGS=-Wall

all: bal_1 bal_2 bal3_cs bal4_cs bal5_cs
	
bal3_cs: bal3_c bal3_s

bal4_cs: bal4_c bal4_s

bal5_cs: bal5_c bal5_s

bal_1: bal_1.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe

bal_2: bal_2.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal3_c: bal3_c.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal3_s: bal3_s.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal4_c: bal4_c.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal4_s: bal4_s.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal5_c: bal5_c.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
bal4_5: bal5_s.c
	$(CC) $^ $(W_FLAGS) -o $(EXE_DIR)$@.exe
	
clean :
	rm -f $(EXE_DIR)*.exe
