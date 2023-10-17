#!/bin/bash

# Set the output file name
output_file="R31_to_R34_added.root"

# Set the input file names
input_files=( ../data/R31_?.root ../data/R32_?.root ../data/R33_?.root ../data/R34_?.root )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"