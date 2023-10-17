#!/bin/bash

# Set the output file name
output_file="R14_to_R19_added.root"

# Set the input file names
input_files=(../data/R14_?.root ../data/R15_?.root ../data/R15_??.root ../data/R16_?.root ../data/R16_??.root ../data/R18_?.root ../data/R18_??.root ../data/R19_?.root  )

# Use hadd to merge the input files into the output file
hadd -T $output_file ${input_files[@]}

# Print a message indicating that the files have been merged
echo "The following files have been merged into $output_file: ${input_files[@]}"