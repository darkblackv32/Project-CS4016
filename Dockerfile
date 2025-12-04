FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libsfml-dev=2.5.1+dfsg-2 \
        libbox2d-dev=2.4.1-2ubuntu1 \
        libasound2 \
        libasound2-plugins \
        libpulse0 && \
    rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN make

CMD ["./game"]


