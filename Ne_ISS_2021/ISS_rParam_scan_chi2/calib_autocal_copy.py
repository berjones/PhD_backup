import os
import sys

# Parameter to vary
param1 = sys.argv[1]
val1 = sys.argv[2]

param2 = sys.argv[3]
val2 = sys.argv[4]

# Make reaction file name
output_filename = f"reactionfiles/calib_{param1}_{val1}_{param2}_{val2}"

# Open the calibration_noarray.txt file and read its contents
with open('reactionfiles/calibration_noarray.txt', 'r') as calibration_file:
    calibration_data = calibration_file.read()

# Open the autocal.txt file and read its contents
with open('autocal_results.cal', 'r') as autocal_file:
    autocal_data = autocal_file.read()

# Append the contents of autocal.txt to calibration_data
combined_data = calibration_data + autocal_data

# If you want to save the combined data to a new file, you can do the following:
with open(output_filename, 'w') as combined_file:
    combined_file.write(combined_data)