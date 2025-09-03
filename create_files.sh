#!/bin/bash

# project root (adjust if needed)
PROJECT_ROOT="concrete"
SRC_DIR="$PROJECT_ROOT/src"
INC_DIR="$PROJECT_ROOT/include"

# verify dirs exist
if [[ ! -d "$SRC_DIR" || ! -d "$INC_DIR" ]]; then
    echo "Error: expected $SRC_DIR and $INC_DIR to exist."
    exit 1
fi

# check if at least one argument is passed
if [ $# -eq 0 ]; then
    echo "Usage: $0 ClassName1 ClassName2 ..."
    exit 1
fi

for NAME in "$@"; do
    CPP_FILE="$SRC_DIR/$NAME.cpp"
    HPP_FILE="$INC_DIR/$NAME.hpp"

    # create .cpp if it doesn't exist
    if [[ ! -f "$CPP_FILE" ]]; then
        echo "#include \"${NAME}.hpp\"" > "$CPP_FILE"
        echo "Created $CPP_FILE"
    else
        echo "Skipped $CPP_FILE (already exists)"
    fi

    # create .hpp if it doesn't exist
    if [[ ! -f "$HPP_FILE" ]]; then
        cat <<EOF > "$HPP_FILE"
#pragma once

class $NAME {
public:
    $NAME();
    ~$NAME();
};
EOF
        echo "Created $HPP_FILE"
    else
        echo "Skipped $HPP_FILE (already exists)"
    fi
done

