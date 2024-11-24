FROM ubuntu:latest

WORKDIR /worker

RUN apt update
RUN apt install netcat-openbsd -y
RUN apt install build-essential -y

COPY main.cpp .

RUN g++ main.cpp -o worker 

ENTRYPOINT ./worker
