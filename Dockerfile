FROM ubuntu:latest as env

RUN apt-get update
RUN apt-get install -y gcc g++ libboost-dev cmake curl valgrind

FROM env as build

COPY . /app

WORKDIR /app/build

RUN cmake ..
RUN cmake --build .

WORKDIR /app

RUN touch out.log
RUN chmod a+rw out.log

RUN groupadd -r sample && useradd -r -g sample sample
USER sample

EXPOSE 8080

ENTRYPOINT valgrind --leak-check=yes ./build/server
