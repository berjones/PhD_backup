import os
import sys

# Parameters to vary
param1 = sys.argv[1]
val1 = sys.argv[2]

param2 = sys.argv[3]
val2 = sys.argv[4]

# Make reaction file name
output_filename = f"reactionfiles/reaction_{param1}_{val1}_{param2}_{val2}"

# Open both files
with open('reactionfiles/reaction_nominal.dat', 'r') as firstfile, open(output_filename, 'w') as secondfile:
    # Read content from the first file
    for line in firstfile:
        # Check if the line contains "Mfield:"
        if line.startswith(param1+':'):
            # If it does, modify the value
            secondfile.write(param1 + ': ' + val1 + '\n')
        elif line.startswith(param2+':'):
            secondfile.write(param2 + ': ' + val2 + '\n')
        else:
            # If not, append the original content to the second file
            secondfile.write(line)