#!/bin/bash

# build: g++ -std=c++11 -o main main.cpp

numbers=(1000 5000 10000)

#create output folder
mkdir -p outputs

# Loop over both arrays simultaneously
for ((i=0; i<${#numbers[@]}; i++)); do
    ./main ${numbers[i]}
done