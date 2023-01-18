# usage: docker run -v <absolute path to zp>:/project snaccoverflow/flash-zp:latest
# lives at https://hub.docker.com/repository/docker/snaccoverflow/flash-zp
FROM ubuntu:20.04

WORKDIR /deps

RUN apt-get update && \
    #apt-get upgrade -y && \
    apt-get install -y \
      build-essential \
      wget && \
    apt-get clean

COPY ~/STMicroelectronics/STM32Cube/STM32CubeProgrammer /deps/cube

# RUN wget https://github.com/stlink-org/stlink/releases/download/v1.7.0/stlink_1.7.0-1_amd64.deb
# RUN apt install -y -f ./stlink_1.7.0-1_amd64.deb