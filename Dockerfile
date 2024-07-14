FROM ubuntu:latest
RUN apt-get update -y && apt-get install make clang-15 clang libgflags-dev git-core clang-format vim -y
RUN cd ~ && git clone --recurse-submodules https://github.com/kemcbride/my_very_good_klondike.git
