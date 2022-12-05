.PHONY: all
all: main tests

.PHONY: main
main:
	mkdir -p bin
	g++ src/*.cpp -o bin/main -Wall -Wextra

.PHONY: tests
tests:
	mkdir -p bin
	g++ tests/*.cpp tests/gtest/gtest-all.cc -Itests/gtest -Isrc -o bin/tests

.PHONY: run-main
run-main: main
	./bin/main

.PHONY: run-tests 
run-tests: tests
	./bin/tests
