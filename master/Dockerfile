FROM ubuntu:latest

WORKDIR /master

RUN apt update
RUN apt install netcat-openbsd -y
RUN apt install build-essential -y

COPY main.cpp .

RUN g++ main.cpp -o master

ENTRYPOINT ./master
