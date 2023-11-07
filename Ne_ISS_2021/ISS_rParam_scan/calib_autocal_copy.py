import os
import sys

# Parameter to vary
param = sys.argv[1]
val = sys.argv[2]

# Make reaction file name
output_filename = f"reactionfiles/calib_{param}_{val}"

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