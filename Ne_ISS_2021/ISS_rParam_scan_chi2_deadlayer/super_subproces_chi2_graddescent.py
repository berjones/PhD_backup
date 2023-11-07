import subprocess
import numpy as np
import pandas as pd

# Define your optimization hyperparameters
learning_rate = 0.01
max_iterations = 100
tolerance = 1e-4

# Read the data from the file
data = pd.read_csv('Ex_vsZ_fit_out.txt', delim_whitespace=True, header=0, names=["param_a", "param_b", "Ex", "m", "m_err", "c", "c_err", "chi2"])

# Extract the numeric value of the first parameter
data['param_a_val'] = data['param_a'].str.split('_').str.get(-1).astype(float)
parameter_name = data['param_a_val'].astype(int).unique()[0]  

# Extract the numeric value of the second parameter
data['param_b_val'] = data['param_b'].str.split('_').str.get(-1).astype(float)
parameter_name = data['param_b_val'].astype(int).unique()[0]  

# Function to get the latest parameters at a specific energy level
def get_latest_params_at_energy(data, energy):
    latest_data_at_energy = data[data['Ex'] == energy].iloc[-1]
    return latest_data_at_energy["param_a_val"], latest_data_at_energy["param_b_val"], latest_data_at_energy["chi2"]

# Access the most recent parameters for the given energy levels (0, 1016.93, 2315.1)
initial_energies = [0, 1016.93, 2315.1]
for energy in initial_energies:
    initial_param_a, initial_param_b, initial_chi2 = get_latest_params_at_energy(data, energy)
    print(f"Energy: {energy}")
    print(f"Initial Param A: {initial_param_a}")
    print(f"Initial Param B: {initial_param_b}")
    print(f"Initial Chi-squared value: {initial_chi2}")


# Gradient descent loop
for iteration in range(max_iterations):

    # Read in the latest parameter values and chi^2 value

    # Choose the next params based on the gradient of the chi2 surface
    
    # Execute your analysis steps using param1 and param2
    param1 = "ArrayDistance"
    val1 = -124
    val1_str = str(val1)

    param2 = "Mfield"
    val2 = 2.03 
    val2_str = str(val2)

    
    subprocess.run('python3 reaction_copy.py ' + param1 + " " + val1_str + param2 + " " + val2_str , shell = True)
    reactionfilename = f"reaction_{param1}_{val1_str}_{param2}_{val2_str}"

    # # iss_sort - sorting data with new reaction file 
    # Run autosort 
    subprocess.run('./autocal_run.sh reactionfiles '+reactionfilename, shell = True)

    # Make the new calibration file
    subprocess.run('python3 calib_autocal_copy.py ' + param1 + " " + val1_str + param2 + " " + val2_str, shell = True)
    calibfilename = f"calib_{param1}_{val1_str}_{param2}_{val2_str}"

    # Making the new hists file
    subprocess.run('./resort.sh reactionfiles ' + reactionfilename + " " + calibfilename, shell = True)

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