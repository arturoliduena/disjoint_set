#!/bin/bash

# Check if the number of arguments is not equal to 1
if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <number>"
    exit 1
fi

number=$1

# build: 
g++ -std=c++11 -o main main.cpp

UFs=("QU" "UR" "UW")
pathCompressions=("NC" "FC" "PS" "PH")
delta=5
#create output folder
mkdir -p outputs

# Define comments
output_file="outputs/UF_${number}.csv"

comment1="# This is a CSV file containing the TPL, TPU,  UF and Compression for the Union-Find data structure."
comment2="# The data was generated for n = $number and delta = $delta ."
echo $comment1 > $output_file
echo $comment2 >> $output_file

# Add header
echo "N,TPL,TPU,UF,path compression." >> $output_file

# Loop over both arrays simultaneously
for ((i=0; i<${#UFs[@]}; i++)); do
    for ((j=0; j<${#pathCompressions[@]}; j++)); do
        echo "Running for ${UFs[i]} with ${pathCompressions[j]} path compression"
        ./main ${UFs[i]} ${pathCompressions[j]} ${number} >> $output_file
    done
done

echo "Data has been written to $output_file"