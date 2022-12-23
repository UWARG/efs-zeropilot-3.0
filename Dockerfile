# usage: docker run -v <absolute path to zp>:/project snaccoverflow/build-zp:latest
# lives at https://hub.docker.com/repository/docker/snaccoverflow/build-zp
FROM ubuntu:18.04

WORKDIR /deps

RUN apt-get update && \
    apt-get upgrade -y && \
    apt-get install -y \
      build-essential \
      git \
      bzip2 \
      cmake \
      wget && \
    apt-get clean
RUN wget -qO- https://developer.arm.com/-/media/Files/downloads/gnu-rm/9-2019q4/gcc-arm-none-eabi-9-2019-q4-major-x86_64-linux.tar.bz2 | tar -xj

ENV PATH "/deps/gcc-arm-none-eabi-9-2019-q4-major/bin:$PATH"

# mount project folder as a volume to this folder
WORKDIR /project

ENTRYPOINT ["./Tools/build.bash", "-c"]