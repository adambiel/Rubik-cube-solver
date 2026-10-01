#!/bin/bash

# Exit immediately if any command fails
set -e

echo "=== Starting project setup ==="

cd src_cpp

# Ensure helper scripts are executable
chmod +x compile.sh link.sh gen_heu_tabs.sh

echo "[1/3] Compiling C++ code..."
./compile.sh

echo "[2/3] Linking the executable..."
./link.sh

echo "[3/3] Generating heuristic tables (this might take a while)..."
./gen_heu_tabs.sh

echo "=== Setup completed successfully! ==="
echo "You can now run the program:"
echo "  python3 main.py"
