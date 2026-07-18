#!/bin/bash

set -e

IMAGE=zelibot-builder

echo "== Construyendo imagen =="

docker build \
  -t $IMAGE \
  .

echo "== Extrayendo binario =="

docker create \
  --name zelibot-temp \
  $IMAGE

docker cp \
  zelibot-temp:/app/build/zelibot \
  ./zelibot

docker rm zelibot-temp

echo "== Listo =="

file ./zelibot
ls -lh ./zelibot
