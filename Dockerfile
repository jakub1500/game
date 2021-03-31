FROM ubuntu:latest

RUN apt-get update
RUN DEBIAN_FRONTEND=noninteractive apt install -y build-essential cmake libncursesw5 libncursesw5-dev 

#libboost-all-dev
#COPY . /opt/source

#RUN mkdir /opt/source/build && cd /opt/source/build && cmake .. && make

# ENTRYPOINT /opt/source/build/server
ENTRYPOINT /usr/bin/bash