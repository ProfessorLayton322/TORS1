FROM ubuntu:latest

RUN apt update
RUN apt install netcat-openbsd -y

ENTRYPOINT nc -luk -p 12101
