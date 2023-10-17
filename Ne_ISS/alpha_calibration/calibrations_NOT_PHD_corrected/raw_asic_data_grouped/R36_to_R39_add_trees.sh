#!/bin/bash

# Set the output file name
output_file="R36_to_R39_added.root"

# Set the input file names
input_files=( ../data/R36_?.root ../data/R37_?.root ../data/R38_?.root ../data/R38_??.root ../data/R39_?.root )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"