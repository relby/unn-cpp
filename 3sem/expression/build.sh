#!/bin/sh

set -xe

mkdir -p ./bin
g++ ./src/*.cpp -o ./bin/main -Wall -Wextra
