#!/bin/bash

# build: 
g++ -std=c++11 -o main main.cpp

numbers=(5000)
UFs=("QU" "UR" "UW")
pathCompressions=("NC" "FC" "PS" "PH")

#create output folder
mkdir -p outputs

# Loop over both arrays simultaneously
for ((i=0; i<${#UFs[@]}; i++)); do
    for ((j=0; j<${#pathCompressions[@]}; j++)); do
        for ((n=0; n<${#numbers[@]}; n++)); do
            ./main ${UFs[i]} ${pathCompressions[j]} ${numbers[n]}
        done
    done
done