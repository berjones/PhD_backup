#!/bin/bash

# Set the output file name
output_file="R43_and_R44_added.root"

# Set the input file names
input_files=( ../data/R43_?.root ../data/R43_??.root ../data/R44_?.root )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"