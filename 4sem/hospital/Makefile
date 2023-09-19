CC = g++
CFLAGS = -std=c++20
OUT_FOLDER = bin
OUT_SERVER = server
OUT_CLIENT = client

all: server client

server:
	mkdir -p $(OUT_FOLDER)
	$(CC) $(CFLAGS) server.cpp -o $(OUT_FOLDER)/$(OUT_SERVER)

client:
	mkdir -p $(OUT_FOLDER)
	$(CC) $(CFLAGS) client.cpp -o $(OUT_FOLDER)/$(OUT_CLIENT)


run-server: server
	./$(OUT_FOLDER)/$(OUT_SERVER)

run-client: client
	./$(OUT_FOLDER)/$(OUT_CLIENT)


