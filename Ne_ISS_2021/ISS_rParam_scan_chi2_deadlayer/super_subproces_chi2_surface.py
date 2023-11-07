import subprocess
import numpy as np
import pandas as pd
import itertools
import random

# Define parameters and their respective arrays of values as a dictionary
param_ranges = {
    "ArrayDeadlayer": [0.0007],
    "Mfield": [2.05]
}

# Generate all possible combinations of parameter values
param_combinations = list(itertools.product(*param_ranges.values()))

# List of combinations that have already been dealt with
dealt_combinations = [
]

# Convert dealt_combinations to a set of tuples for efficient comparison
dealt_set = set(dealt_combinations)

# Filter out combinations that have already been dealt with
param_combinations = [comb for comb in param_combinations if comb not in dealt_set]

# Shuffle the list of combinations
random.shuffle(param_combinations)

for param_values in param_combinations:
    param_dict = dict(zip(param_ranges.keys(), param_values))

    # Extracting parameter names and values dynamically
    param1, val1 = list(param_dict.items())[0]
    param2, val2 = list(param_dict.items())[1]

    val1_str = str(val1)
    val2_str = str(val2)
    
    # Note the start time of this iteration
    # subprocess.run('python3 time_recorder.py ', shell = True)

    # # Make the new reaction file 
    # subprocess.run('python3 reaction_copy.py ' + param1 + " " + val1_str + " " + param2 + " " + val2_str , shell = True)
    reactionfilename = f"reaction_{param1}_{val1_str}_{param2}_{val2_str}"
 
    # # iss_sort - sorting data with new reaction file 
    # Run autosort 
    # subprocess.run('./autocal_run.sh reactionfiles '+reactionfilename, shell = True)

    # Make the new calibration file
    # subprocess.run('python3 calib_autocal_copy.py ' + param1 + " " + val1_str + " " + param2 + " " + val2_str, shell = True)
    # calibfilename = f"calib_{param1}_{val1_str}_{param2}_{val2_str}"

    # Making the new hists file
    # subprocess.run('./resort_parallel.sh reactionfiles ' + reactionfilename + " " + calibfilename, shell = True)

    # Take the sorted hists file and make the projections
    makeprojections = 'root -l -b -q "Ex_projector.cc(\\"histfiles/' + reactionfilename + '.root\\")"'
    subprocess.run(makeprojections, shell = True)

    # Make the batch files for the projections 
    projname = f"proj_reaction_{param1}_{val1}_{param2}_{val2}.root"
    subprocess.run('python3 batch_copy.py ' + projname, shell = True)

    # Fit the desired peaks with hdtv in batch mode - will require a new batch file for each peak 
    subprocess.run('./hdtv_run.sh', shell = True)

    # Plot the Ex vs z for the states in question
    paramvals1 = param1 + "_" + val1_str 
    paramvals2 = param2 + "_" + val2_str
    makeEvsZ = 'root -l -b -q "run_Ex_vs_z.C(\\"' + paramvals1 + '\\", \\"' + paramvals2 + '\\")"'
    subprocess.run(makeEvsZ, shell = True)

    # Make my lovely Chi2 surface
    subprocess.run('python3 chi2_surface.py ', shell = True)






# # Calculate chi-squared value (chi2_0_ndf) for the current parameter combination

# # Update parameters using gradient descent
# param_a -= learning_rate * gradient_a
# param_b -= learning_rate * gradient_b

# # Calculate the new chi-squared value
# current_chi2 = calculate_chi2(param_a, param_b)

# # Check for convergence
# if abs(current_chi2 - initial_chi2) < tolerance:
#     break

# initial_chi2 = current_chi2