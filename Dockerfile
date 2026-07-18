FROM debian:13

RUN apt update && apt install -y \
    cmake \
    g++ \
    git \
    make \
    libssl-dev \
    libcurl4-openssl-dev \
    libboost-system-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . .

RUN cmake -B build -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build -j$(nproc)
