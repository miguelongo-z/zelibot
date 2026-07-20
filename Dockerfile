
FROM debian:bookworm-slim AS builder

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends build-essential cmake git ca-certificates libboost-all-dev libssl-dev zlib1g-dev libcurl4-openssl-dev && rm -rf /var/lib/apt/lists/*

COPY . .

RUN make all

FROM debian:bookworm-slim

WORKDIR /app

RUN apt-get update && apt-get install -y --no-install-recommends libboost-system1.74.0 libssl3 zlib1g libcurl4 && rm -rf /var/lib/apt/lists/*

COPY --from=builder /app/build/zelibot ./zelibot

CMD ["./zelibot"]
