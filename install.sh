#!/bin/bash

# List of dependencies
dependencies=("gcc" "curl" "unzip" "gh")

# Loop through the dependencies
for dependency in "${dependencies[@]}"; do
    if ! command -v "$dependency" &> /dev/null; then
        echo "$dependency is not installed. Please install it first."
        exit 1
    fi
done

# Compile the main.c and src files
curl -o cdeps.zip https://github.com/danidomi/cdeps/archive/refs/heads/main.zip

#unzip it
unzip -q cdeps.zip -d cdeps && rm cdeps.zip

# cd a directory
cd cdeps

src=$(find src -type f -name "*.c")
gcc -o cdeps *.c $src -w

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Move the executable to a directory in your PATH (e.g., /usr/local/bin)
    mkdir -p ~/.local/bin
    mv cdeps ~/.local/bin/cdeps
else
    echo "Compilation failed. Please check for errors in your C code."
fi

rm -rf cdeps

export PATH="$HOME/.local/bin:$PATH"