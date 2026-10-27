#!/bin/bash
N=100000000
for p in 1 2 4 8; do
  echo "========== HILOS: $p =========="
  for i in 1 2 3 4 5; do
    echo "--- Ejecucion $i ---"
    ./montecarlo_paralelo $N $p
  done
done
