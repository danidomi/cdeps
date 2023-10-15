#!/bin/bash

# List of dependencies
dependencies=("gcc" "curl" "unzip")

# Loop through the dependencies
for dependency in "${dependencies[@]}"; do
    if ! command -v "$dependency" &> /dev/null; then
        echo "$dependency is not installed. Please install it first."
        exit 1
    fi
done

src=$(find src -type f -name "*.c")

# Compile the main.c and src files
gcc -o cdeps *.c $src -w

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Move the executable to a directory in your PATH (e.g., /usr/local/bin)
    mkdir -p ~/.local/bin
    mv cdeps ~/.local/bin/cdeps
else
    echo "Compilation failed. Please check for errors in your C code."
fi

export PATH="$HOME/.local/bin:$PATH"