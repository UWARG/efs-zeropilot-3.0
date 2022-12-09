# usage: docker run -v <absolute path to zp>:/project snaccoverflow/test-zp:latest
# lives at https://hub.docker.com/repository/docker/snaccoverflow/test-zp
FROM ubuntu:18.04

WORKDIR /deps

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
      build-essential \
      software-properties-common \
      wget \
      libgtest-dev && \
    apt-get clean

RUN wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | apt-key add -
RUN apt-add-repository 'deb https://apt.kitware.com/ubuntu/ bionic main' && \
    apt-get update && \
    apt install -y cmake && \
    apt-get clean

WORKDIR /usr/src/gtest
RUN cmake .
RUN make
RUN cp *.a /usr/lib

# mount project folder as a volume to this folder
WORKDIR /project

CMD ./testing/test.bash build && ./testing/test.bash run 