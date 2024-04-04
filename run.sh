#!/bin/bash

# Using command line argument if provided, otherwise prompt with read
if [ "$1" ]; then
    file_name="$1"
else
    echo "Enter your file_name:"
    read file_name
fi

g++ -std=c++11 -o p main $file_name
