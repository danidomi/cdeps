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

plugin_sources=$(find plugins -type f -name "*.c")

# Compile the cdeps.c source file and all plugins
gcc -o cdeps *.c $plugin_sources

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Move the executable to a directory in your PATH (e.g., /usr/local/bin)
    mkdir -p ~/.local/bin
    mv cdeps ~/.local/bin/cdeps
else
    echo "Compilation failed. Please check for errors in your C code."
fi
