#!/bin/bash

# Set the output file name
output_file="R40_and_R41_added.root"

# Set the input file names
input_files=( ../data/R40_?.root ../data/R40_??.root ../data/R41_?.root ../data/R41_??.root)

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"