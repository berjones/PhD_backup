#!/bin/bash

# Set the output file name
output_file="R4_to_R9_added.root"

# Set the input file names
input_files=(../data/R4_?.root ../data/R5_?.root ../data/R7_?.root ../data/R8_?.root ../data/R9_?.root)

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"