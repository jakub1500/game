FROM ubuntu:latest

RUN apt update
RUN DEBIAN_FRONTEND=noninteractive apt install -y build-essential cmake libncursesw5 libncursesw5-dev libboost-all-dev

COPY . /opt/source

ENTRYPOINT /bin/bash 