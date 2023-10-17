#!/bin/bash

# Set the output file name
output_file="R21_to_R26_added.root"

# Set the input file names
input_files=( ../data/R21_?.root ../data/R22_?.root ../data/R23_?.root ../data/R24_?.root ../data/R24_??.root ../data/R25_?.root ../data/R26_?.root )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"