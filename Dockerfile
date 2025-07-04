FROM ubuntu:latest
RUN apt-get update -y && apt-get install make git-core cmake clang-14 clang clang-format libgflags-dev libgtest-dev vim -y
RUN cd ~ && git clone --recurse-submodules https://github.com/kemcbride/my_very_good_klondike.git
