#!/bin/bash

# Root directory containing the libraries
root_dir="/vlc"
lib_dir="$root_dir/build/linux_64/lib"
patchelf="$root_dir/patchelf"

# Function for concatenating paths
add_to_path() {
    local path1="$1"
    local path2="$2"

    if [[ -n "$path1" ]]; then
        path1="$path1:$path2"
    else
        path1="$path2"
    fi

    echo "$path1"
}

# Function to convert an absolute path to a relative path with $ORIGIN
convert_to_origin() {
    local lib_path="$1"
    local rpath="$2"
    local relative_rpath=""

    # Split rpath by ':' and process each path
    local IFS=":"
    for path in $rpath; do
        # If path is already with '$ORIGIN', skip
        if [[ $path == *"\$ORIGIN"* ]]; then
            relative_rpath=$(add_to_path "$relative_rpath" "$path")
            continue
        fi
        # Calculate the relative path
        rel_path=$(realpath --relative-to="$(dirname "$lib_path")" "$path")
        relative_rpath=$(add_to_path "$relative_rpath" "\$ORIGIN/$rel_path")
    done

    echo "$relative_rpath"
}

# Find all shared libraries and update their RPATH
libs=$(find "$lib_dir" -name '*.so*' -type f)
for lib in $libs; do
    current_rpath=$($patchelf --print-rpath "$lib")
    if [[ -n "$current_rpath" ]]; then
        new_rpath=$(convert_to_origin "$lib" "$current_rpath")
        echo "Updating RPATH of $lib to $new_rpath"
        $patchelf --set-rpath "$new_rpath" "$lib"
    fi
done
