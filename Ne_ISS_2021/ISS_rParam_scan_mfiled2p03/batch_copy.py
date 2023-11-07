import os
import sys

# Parameter to vary
param = sys.argv[1]

# Make reaction file name
output_filename1 = f"batchfiles/batch_E0.txt"
output_filename2 = f"batchfiles/batch_E1016.txt"
output_filename3 = f"batchfiles/batch_E2315.txt"


# Open both files for batch 1 
with open('batchfiles_backup/batch_E0.txt', 'r') as firstfile, open(output_filename1, 'w') as secondfile:
    # Read content from the first file
    for line in firstfile:
        # Check if the line contains "Mfield:"
        if line.startswith('root'):
            # If it does, modify the value
            secondfile.write("root get projections/" + param + "/*" +'\n')
        else:
            # If not, append the original content to the second file
            secondfile.write(line)

# Open both files for batch 2
with open('batchfiles_backup/batch_E1016.txt', 'r') as firstfile, open(output_filename2, 'w') as secondfile:
    # Read content from the first file
    for line in firstfile:
        # Check if the line contains "Mfield:"
        if line.startswith('root'):
            # If it does, modify the value
            secondfile.write("root get projections/" + param + "/*" +'\n')
        else:
            # If not, append the original content to the second file
            secondfile.write(line)

# Open both files for batch 3
with open('batchfiles_backup/batch_E2315.txt', 'r') as firstfile, open(output_filename3, 'w') as secondfile:
    # Read content from the first file
    for line in firstfile:
        # Check if the line contains "Mfield:"
        if line.startswith('root'):
            # If it does, modify the value
            secondfile.write("root get projections/" + param + "/*" +'\n')
        else:
            # If not, append the original content to the second file
            secondfile.write(line)           