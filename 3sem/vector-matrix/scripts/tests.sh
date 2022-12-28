#!/bin/sh

set -xe

g++ test*.cpp ../gtest/gtest-all.cc -I../gtest -o ./bin/tests
