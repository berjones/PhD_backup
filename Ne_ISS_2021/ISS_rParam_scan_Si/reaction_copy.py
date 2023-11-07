import os
import sys

# Parameter to vary
param = sys.argv[1]
val = sys.argv[2]

# Make reaction file name
output_filename = f"reactionfiles/reaction_{param}_{val}"

# Open both files
with open('reactionfiles/reaction_nominal.dat', 'r') as firstfile, open(output_filename, 'w') as secondfile:
    # Read content from the first file
    for line in firstfile:
        # Check if the line contains "Mfield:"
        if line.startswith(param+':'):
            # If it does, modify the value
            secondfile.write(param + ': ' + val + '\n')
        else:
            # If not, append the original content to the second file
            secondfile.write(line)