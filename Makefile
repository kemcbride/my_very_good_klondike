CPUS ?= $(shell (nproc --all || sysctl -n hw.ncpu) 2>/dev/null || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

.PHONY: all clean test tidy solitaire dbg_solitaire test_gtest test_e2e

all: solitaire test_solitaire run

clean:
	bazel clean

run: solitaire
	bazel run :solitaire play

test: test_gtest dbg_solitaire solitaire
	./full_solve_tests.sh

tidy:
	clang-format -i bin/*.cpp lib/*.h lib/*.cpp test/*.cpp benchmark/*.cpp --style=google

solitaire:
	CC=clang++ bazel build --platform_suffix=clang-opt --copt -O3 :solitaire

dbg_solitaire:
	CC=clang++ bazel build -c dbg --platform_suffix=clang-dbg :solitaire

test_gtest:
	bazel test gtest
	bazel run gtest

test_e2e: solitaire dbg_solitaire
	./full_solve_tests.sh

isstuck_benchmark: benchmark/isstuck_benchmark.cpp $(objects)
	$(CC) -v $< $(objects) -lbenchmark $(CC_FLAGS) -o $@ 

