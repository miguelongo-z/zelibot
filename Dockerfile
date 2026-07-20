FROM debian:stable-slim 

WORKDIR /app

RUN apt-get update && apt-get install -y build-essential cmake libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev g++ gcc git

COPY . .

RUN make all 

CMD ["./build/zelibot"]


