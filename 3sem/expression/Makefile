.PHONY: all
all: main tests

.PHONY: main
main:
	mkdir -p bin
	g++ src/main.cpp src/implementations/*.cpp -o bin/main -Wall -Wextra -Isrc/include

.PHONY: tests
tests:
	mkdir -p bin
	g++ src/implementations/*.cpp tests/*.cpp tests/gtest/gtest-all.cc -Itests/gtest -Isrc/include -o bin/tests

.PHONY: run-main
run-main: main
	./bin/main

.PHONY: run-tests 
run-tests: tests
	./bin/tests
