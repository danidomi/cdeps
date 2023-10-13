#!/bin/bash

# Check if the necessary build tools are installed (you can add more as needed)
if ! command -v gcc &> /dev/null; then
    echo "gcc is not installed. Please install it first."
    exit 1
fi

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

export PATH="$HOME/.local/bin:$PATH" >> ~/.zshrc
echo "> source ~/.zshrc"
