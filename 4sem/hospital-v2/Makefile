CC = g++
CFLAGS = -std=c++20
OUT_FOLDER = bin
OUT_SERVER = server.out
OUT_CLIENT = client.out

all: server client

bin:
	mkdir -p $(OUT_FOLDER)

server: bin
	$(CC) $(CFLAGS) src/server/main.cpp -lpthread -o $(OUT_FOLDER)/$(OUT_SERVER)

client: bin
	$(CC) $(CFLAGS) src/client/main.cpp -o $(OUT_FOLDER)/$(OUT_CLIENT)

run-server: server
	./$(OUT_FOLDER)/$(OUT_SERVER)

run-client: client
	./$(OUT_FOLDER)/$(OUT_CLIENT)
