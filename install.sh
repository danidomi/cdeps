#!/bin/bash

set -e

# Define the application name
app_name="cdeps"

# repo
branch="main"

# List of dependencies
dependencies=("gcc" "curl" "unzip")

# Loop through the dependencies
for dependency in "${dependencies[@]}"; do
    if ! command -v "$dependency" &> /dev/null; then
        echo "$dependency is not installed. Please install it first."
        exit 1
    fi
done

# Work in a temp directory so we don't pollute the user's cwd
tmp_dir=$(mktemp -d)
trap 'rm -rf "$tmp_dir"' EXIT
cd "$tmp_dir"

# Download
curl -fsSL -o "$app_name.zip" "https://github.com/danidomi/$app_name/archive/refs/heads/$branch.zip"

# Unzip it
unzip -q "$app_name.zip"

# cd into the extracted directory
cd "$app_name-$branch"

# Compile main.c and src files
src=$(find src -type f -name "*.c")
gcc -o "$app_name" main.c $src -w

# Move the executable to a directory in the user's PATH
mkdir -p ~/.local/bin
mv "$app_name" ~/.local/bin/"$app_name"

echo "Installed $app_name to ~/.local/bin/$app_name"
echo "Make sure ~/.local/bin is in your PATH:"
echo '  export PATH="$HOME/.local/bin:$PATH"'
