#!/bin/bash

script_name=$(basename "$0")

# Kompilieren
cd ..
bash compile.sh
cd testumgebung

rm -v v_*

for file in *; do
    # Skip directories and the script itself
    if [[ "$file" != "$script_name" && -f "$file" ]]; then
        output_file="v_$file"
        echo ''
        ./Verwirrrer "$file" -o "$output_file"
    fi
done
