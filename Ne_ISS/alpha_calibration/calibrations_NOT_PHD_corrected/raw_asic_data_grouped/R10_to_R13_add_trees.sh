#!/bin/bash

# Set the output file name
output_file="R10_to_R13_added.root"

# Set the input file names
input_files=(../data/R10_?.root ../data/R11_?.root ../data/R12_?.root ../data/R12_1?.root ../data/R13_?.root )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"