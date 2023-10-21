#!/bin/bash

# Define the application name
app_name="cdeps"

# repo
branch = "main"

# List of dependencies
dependencies=("gcc" "curl" "unzip")

# Loop through the dependencies
for dependency in "${dependencies[@]}"; do
    if ! command -v "$dependency" &> /dev/null; then
        echo "$dependency is not installed. Please install it first."
        exit 1
    fi
done

# Compile the main.c and src files
wget https://github.com/danidomi/"$app_name"/archive/refs/heads/"$branch".zip -O "$app_name".zip

#unzip it
unzip "$app_name".zip

# cd a directory
cd "$app_name"-"$branch"

src=$(find src -type f -name "*.c")
gcc -o "$app_name" *.c $src -w

# Check if the compilation was successful
if [ $? -eq 0 ]; then
    # Move the executable to a directory in your PATH (e.g., /usr/local/bin)
    mkdir -p ~/.local/bin
    mv "$app_name" ~/.local/bin/"$app_name"
else
    echo "Compilation failed. Please check for errors in your C code."
fi

rm -rf "$app_name"

export PATH="$HOME/.local/bin:$PATH"