import subprocess 
import numpy as np 

# # Make my new reaction file
param = "Mfield"

# Single value of val = "2.0"
# Loop for value that youre investigating
for val in np.arange(1.9, 2.11, 0.05):
    val_str = str(val)
    subprocess.run('python3 reaction_copy.py ' + param + " " + val_str, shell = True)
    reactionfilename = f"reaction_{param}_{val_str}"

    # # iss_sort - sorting data with new reaction file 
    # Run autosort 
    subprocess.run('./autocal_run.sh reactionfiles '+reactionfilename, shell = True)

    # Make the new calibration file
    subprocess.run('python3 calib_autocal_copy.py ' + param + " " + val_str, shell = True)
    calibfilename = f"calib_{param}_{val_str}"

    # Making the new hists file
    subprocess.run('./resort.sh reactionfiles ' + reactionfilename + " " + calibfilename, shell = True)

    # Take the sorted hists file and make the projections
    makeprojections = 'root -l -b -q "Ex_projector.cc(\\"histfiles/' + reactionfilename + '.root\\")"'
    subprocess.run(makeprojections, shell = True)

    # Make the batch files for the projections 
    projname = f"proj_reaction_{param}_{val}.root"
    subprocess.run('python3 batch_copy.py ' + projname, shell = True)

    # Fit the desired peaks with hdtv in batch mode - will require a new batch file for each peak 
    subprocess.run('./hdtv_run.sh', shell = True)

    # Plot the Ex vs z for the states in question
    paramvals = param + "_" + val_str
    makeEvsZ = 'root -l -b -q "run_Ex_vs_z.C(\\"'+ paramvals +'\\")"'
    subprocess.run(makeEvsZ, shell = True)

