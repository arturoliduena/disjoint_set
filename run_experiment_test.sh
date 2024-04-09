#!/bin/bash

g++ -std=c++11 -o main main.cpp

UFS=("QU_NC" "QU_FC" "UR_FC" "UR_NC" "UW_FC" "UW_NC")
numbers=(1000 5000 10000)

# Loop over both arrays simultaneously
for ((i=0; i<${#numbers[@]}; i++)); do
    for ((j=0; j<${#UFS[@]}; j++)); do
        echo "${UFS[j]} ${numbers[i]}"
        ./main ${UFS[j]} ${numbers[i]}
    done
done