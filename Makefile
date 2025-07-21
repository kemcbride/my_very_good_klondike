CC_FLAGS=-Wall -Wextra -pthread -I. --std=c++20 -stdlib=libc++

CPUS ?= $(shell (nproc --all || sysctl -n hw.ncpu) 2>/dev/null || echo 1)
MAKEFLAGS += --jobs=$(CPUS)

.PHONY: all clean test tidy profile solitaire dbg_solitaire prof_solitaire test_gtest test_e2e

all: solitaire test_solitaire run

clean:
	rm -f *.o *.a objects/**/*.o solitaire gtest dbg_solitaire prof_solitaire \
		gprof.out gmon.out

run: solitaire
	bazel run :solitaire play

test: test_gtest dbg_solitaire solitaire
	./full_solve_tests.sh

tidy:
	clang-format -i bin/*.cpp lib/*.h lib/*.cpp test/*.cpp --style=google

solitaire:
	CC=clang++ bazel build --platform_suffix=clang-opt --copt -O3 :solitaire

dbg_solitaire:
	CC=clang++ bazel build -c dbg --platform_suffix=clang-dbg :solitaire

prof_solitaire:
	CC=clang++ bazel build -c dbg --platform_suffix=clang-prof --copt -pg :solitaire

test_gtest:
	bazel test gtest
	bazel run gtest

test_e2e: solitaire dbg_solitaire
	./full_solve_tests.sh

isstuck_benchmark: benchmark/isstuck_benchmark.cpp $(objects)
	$(CC) -v $< $(objects) -lbenchmark $(CC_FLAGS) -o $@ 

profile: prof_solitaire
	./prof_solitaire play 4 < inputs/seed4_another_input.txt
	gprof ./prof_solitaire > gprof.out
