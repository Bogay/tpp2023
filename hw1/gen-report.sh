#!/usr/bin/bash

file_header() {
    echo "/* -------------------------------------------------------------"
    echo "// $1"
    echo "// ----------------------------------------------------------- */"
}

for file in "$@"; do
    header="$(file_header "$file")"
    echo "$header"
    echo ""
    cat "$file"
    echo ""
done
